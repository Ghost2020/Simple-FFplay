#include "FMediaPlayer.h"

#include <cmath>
#include <functional>
#include <inttypes.h>

extern "C"
{
#include "libavutil/dict.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#ifdef CONFIG_AVFILTER
#include "libavutil/avstring.h"
#include "libavutil/pixdesc.h"
#endif
}

#if defined(__linux__)
//#include <QWindow>
//#include "X11/X.h"
extern "C"
{
    #include "gtk/gtk.h"
}
#endif

constexpr uint32_t MAX_QUEUE_SIZE = (15 * 1024 * 1024);
constexpr uint32_t MIN_FRAMES = 25;
constexpr uint32_t EXTERNAL_CLOCK_MIN_FRAMES = 2;
constexpr uint32_t EXTERNAL_CLOCK_MAX_FRAMES = 10;

/* Minimum SDL audio buffer size, in samples. */
constexpr uint32_t SDL_AUDIO_MIN_BUFFER_SIZE = 512;
/* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
constexpr uint32_t SDL_AUDIO_MAX_CALLBACKS_PER_SEC = 30;

/* no AV sync correction is done if below the minimum AV sync threshold */
constexpr double AV_SYNC_THRESHOLD_MIN = 0.04;
/* AV sync correction is done if above the maximum AV sync threshold */
constexpr double AV_SYNC_THRESHOLD_MAX = 0.1;
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
constexpr double AV_SYNC_FRAMEDUP_THRESHOLD = 0.1;
/* no AV correction is done if too big error */
constexpr double AV_NOSYNC_THRESHOLD = 10.0;

/* maximum audio speed change to get correct sync */
constexpr uint32_t SAMPLE_CORRECTION_PERCENT_MAX = 10;

/* external clock speed adjustment constants for realtime sources based on buffer fullness */
constexpr double EXTERNAL_CLOCK_SPEED_MIN = 0.900;
constexpr double EXTERNAL_CLOCK_SPEED_MAX = 1.010;
constexpr double EXTERNAL_CLOCK_SPEED_STEP = 0.001;

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
constexpr uint32_t AUDIO_DIFF_AVG_NB = 20;

constexpr uint32_t USE_ONEPASS_SUBTITLE_RENDER = 1;

static const struct TextureFormatEntry {
    enum AVPixelFormat format;
    int texture_fmt;
} sdl_texture_format_map[] = {
    { AV_PIX_FMT_RGB8,           SDL_PIXELFORMAT_RGB332 },
    { AV_PIX_FMT_RGB444,         SDL_PIXELFORMAT_RGB444 },
    { AV_PIX_FMT_RGB555,         SDL_PIXELFORMAT_RGB555 },
    { AV_PIX_FMT_BGR555,         SDL_PIXELFORMAT_BGR555 },
    { AV_PIX_FMT_RGB565,         SDL_PIXELFORMAT_RGB565 },
    { AV_PIX_FMT_BGR565,         SDL_PIXELFORMAT_BGR565 },
    { AV_PIX_FMT_RGB24,          SDL_PIXELFORMAT_RGB24 },
    { AV_PIX_FMT_BGR24,          SDL_PIXELFORMAT_BGR24 },
    { AV_PIX_FMT_0RGB32,         SDL_PIXELFORMAT_RGB888 },
    { AV_PIX_FMT_0BGR32,         SDL_PIXELFORMAT_BGR888 },
    { AV_PIX_FMT_NE(RGB0, 0BGR), SDL_PIXELFORMAT_RGBX8888 },
    { AV_PIX_FMT_NE(BGR0, 0RGB), SDL_PIXELFORMAT_BGRX8888 },
    { AV_PIX_FMT_RGB32,          SDL_PIXELFORMAT_ARGB8888 },
    { AV_PIX_FMT_RGB32_1,        SDL_PIXELFORMAT_RGBA8888 },
    { AV_PIX_FMT_BGR32,          SDL_PIXELFORMAT_ABGR8888 },
    { AV_PIX_FMT_BGR32_1,        SDL_PIXELFORMAT_BGRA8888 },
    { AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_IYUV },
    { AV_PIX_FMT_YUYV422,        SDL_PIXELFORMAT_YUY2 },
    { AV_PIX_FMT_UYVY422,        SDL_PIXELFORMAT_UYVY },
    { AV_PIX_FMT_NONE,           SDL_PIXELFORMAT_UNKNOWN },
};

constexpr uint32_t FF_QUIT_EVENT = (SDL_USEREVENT + 2);

void SClock::Init(int* queue_serial)
{
    this->speed = 1.0;
    this->paused = false;
    this->queue_serial = queue_serial;
    this->Set(NAN, -1);
}

double SClock::Get()
{
    if (*queue_serial != serial)
        return NAN;
    if (paused)
        return pts;
    else {
        double time = av_gettime_relative() / 1000000.0;
        return pts_drift + time - (time - last_updated) * (1.0 - speed);
    }
}

void SClock::SetAt(double pts, int serial, double time)
{
    this->pts = pts;
    this->last_updated = time;
    this->pts_drift = pts - time;
    this->serial = serial;
}

void SClock::Set(double pts, int serial)
{
    double time = av_gettime_relative() / 1000000.0;
    this->SetAt(pts, serial, time);
}

void SClock::SetSpeed(double speed)
{
    this->Set(Get(), serial);
    this->speed = speed;
}

uint8_t FMediaPlayer::g_nInstance = 0;
std::mutex FMediaPlayer::g_mutex;
static FMediaPlayer::ESyncType g_av_sync_type = FMediaPlayer::ESyncType::AV_SYNC_AUDIO_MASTER;

FMediaPlayer::EShowMode FMediaPlayer::eShow_mode = FMediaPlayer::EShowMode::SHOW_MODE_VIDEO;

FMediaPlayer::FMediaPlayer(uint64_t windowID)
    :m_nWindowID(windowID)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    if (++g_nInstance == 1)
        initContext();
};

FMediaPlayer::~FMediaPlayer()
{
    OnExit();

    if (--g_nInstance == 0)
        unInitContext();
}

bool FMediaPlayer::initContext()
{
    av_log_set_flags(AV_LOG_PRINT_LEVEL);

    /* register all codecs, demux and protocols */
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avformat_network_init();

    /*-----------------------------------???????????---------------------------------------*/

    int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;

    /* Try to work around an occasional ALSA buffer underflow issue when the
     * period size is NPOT due to ALSA resampling by forcing the buffer size. */
    if (!SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE"))
        SDL_setenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE", "1", 1);

    if (SDL_Init(flags))
    {
        av_log(nullptr, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        av_log(nullptr, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
        return false;
    }

    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
    /*-----------------------------------???????????---------------------------------------*/

#ifdef __linux__
    /* signal capture */
    signal(SIGSEGV, [] (int signum) -> void
    {
        static uint32_t count = 0;
        if((++count / 100) == 0)
            av_log(nullptr, AV_LOG_ERROR, "Accept a signal:%d\n", signum);
    });
#endif

    return true;
}

void FMediaPlayer::unInitContext()
{
    avformat_network_deinit();

    SDL_Quit();
}

bool FMediaPlayer::initRender()
{
    int flags = SDL_WINDOW_HIDDEN;

    flags |= SDL_WINDOW_RESIZABLE;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    //SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, "");
    if(m_nWindowID)
    {
#if defined(__APLLE__) // Q_MAC_USE_COCOA
        NSView* view = reinterpret_cast<NSView*>(m_nWindowID);
        //NSWindow* wnd = [view window];
        pWindow = SDL_CreateWindowFrom((void*)(view.window));
#elif defined(__WIN32__)
        pWindow = SDL_CreateWindowFrom((void*)(m_nWindowID));
#endif
    }
    else
    {
        pWindow = SDL_CreateWindow((std::string("FFmplay") + std::to_string(g_nInstance)).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, default_width, default_height, flags);
    }


    if (pWindow)
    {
        pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!pRenderer) {
            av_log(nullptr, AV_LOG_WARNING, "Failed to initialize a hardware accelerated renderer: %s\n", SDL_GetError());
            pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        }
        if (pRenderer) {
            if (!SDL_GetRendererInfo(pRenderer, &renderer_info))
                av_log(nullptr, AV_LOG_VERBOSE, "Initialized %s renderer.\n", renderer_info.name);
        }
    }
    if (!pWindow || !pRenderer || !renderer_info.num_texture_formats)
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to create window or renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

void FMediaPlayer::uninitRender()
{
    if (this->vis_texture)
        SDL_DestroyTexture(this->vis_texture);
    if (this->vid_texture)
        SDL_DestroyTexture(this->vid_texture);
    if (this->sub_texture)
        SDL_DestroyTexture(this->sub_texture);

    if (pRenderer)
        SDL_DestroyRenderer(pRenderer);
    if (pWindow)
        SDL_DestroyWindow(pWindow);
}

void FMediaPlayer::OnWindowSizeChange(int width, int height, AVRational sar)
{
    int max_width = screen_width ? screen_width : INT_MAX;
    int max_height = screen_height ? screen_height : INT_MAX;
    if (max_width == INT_MAX && max_height == INT_MAX)
        max_height = height;

    SDL_Rect rect;
    calculate_display_rect(rect, SDL_Rect{ 0, 0, max_width, max_height }, width, height, sar);
    default_width = rect.w;
    default_height = rect.h;
}

void FMediaPlayer::fill_rectangle(int nXPos, int nYPos, int nWidth, int nHeight)
{
    SDL_Rect rect{ nXPos, nYPos , nWidth , nHeight };
    if (nWidth && nHeight)
        SDL_RenderFillRect(pRenderer, &rect);
}

bool FMediaPlayer::realloc_texture(SDL_Texture** texture, Uint32 nNewFormat, int nNewWidth, int nNewHeight, SDL_BlendMode eBlendmode, bool bInitTexture)
{
    Uint32 format;
    int access, w, h;
    if (!*texture || SDL_QueryTexture(*texture, &format, &access, &w, &h) < 0 || nNewWidth != w || nNewHeight != h || nNewFormat != format)
    {
        void* pixels = nullptr;
        int pitch;
        if (*texture)
            SDL_DestroyTexture(*texture);
        if (!(*texture = SDL_CreateTexture(pRenderer, nNewFormat, SDL_TEXTUREACCESS_STREAMING, nNewWidth, nNewHeight)))
            return false;
        if (SDL_SetTextureBlendMode(*texture, eBlendmode) < 0)
            return false;
        if (bInitTexture)
        {
            if (SDL_LockTexture(*texture, nullptr, &pixels, &pitch) < 0)
                return false;
            memset(pixels, 0, pitch * nNewHeight);
            SDL_UnlockTexture(*texture);
        }
        av_log(nullptr, AV_LOG_VERBOSE, "Created %dx%d texture with %s.\n", nNewWidth, nNewHeight, SDL_GetPixelFormatName(nNewFormat));
    }
    return true;
}

void FMediaPlayer::calculate_display_rect(SDL_Rect& rect, const SDL_Rect& srcRect,
    int pic_width, int pic_height, AVRational pic_sar)
{
    AVRational aspect_ratio = pic_sar;
    int64_t width, height, x, y;

    if (av_cmp_q(aspect_ratio, av_make_q(0, 1)) <= 0)
        aspect_ratio = av_make_q(1, 1);

    aspect_ratio = av_mul_q(aspect_ratio, av_make_q(pic_width, pic_height));

    /* XXX: we suppose the screen has a 1.0 pixel ratio */
    height = srcRect.h;
    width = av_rescale(height, aspect_ratio.num, aspect_ratio.den) & ~1;
    if (width > srcRect.w) {
        width = srcRect.w;
        height = av_rescale(width, aspect_ratio.den, aspect_ratio.num) & ~1;
    }
    x = (srcRect.w - width) / 2;
    y = (srcRect.h - height) / 2;
    rect.x = srcRect.x + x;
    rect.y = srcRect.y + y;
    rect.w = FFMAX((int)width, 1);
    rect.h = FFMAX((int)height, 1);
}

void FMediaPlayer::get_sdl_pix_fmt_and_blendmode(int format, Uint32* sdl_pix_fmt, SDL_BlendMode* sdl_blendmode)
{
    *sdl_blendmode = SDL_BLENDMODE_NONE;
    *sdl_pix_fmt = SDL_PIXELFORMAT_UNKNOWN;
    if (format == AV_PIX_FMT_RGB32 ||
        format == AV_PIX_FMT_RGB32_1 ||
        format == AV_PIX_FMT_BGR32 ||
        format == AV_PIX_FMT_BGR32_1)
        *sdl_blendmode = SDL_BLENDMODE_BLEND;

    for (uint32_t i = 0; i < FF_ARRAY_ELEMS(sdl_texture_format_map) - 1; i++) {
        if (format == sdl_texture_format_map[i].format) {
            *sdl_pix_fmt = sdl_texture_format_map[i].texture_fmt;
            return;
        }
    }
}

bool FMediaPlayer::upload_texture(SDL_Texture** tex, AVFrame* frame, struct SwsContext** img_convert_ctx)
{
    int ret = -1;
    Uint32 sdl_pix_fmt;
    SDL_BlendMode sdl_blendmode;

    /*if(frame->linesize[0] > 0 && frame->linesize[1] > 0 && frame->linesize[2] > 0)
        m_Rendercallback(frame->data, frame->width, frame->height);

    return true;*/
    get_sdl_pix_fmt_and_blendmode(frame->format, &sdl_pix_fmt, &sdl_blendmode);
    if (!realloc_texture(tex, sdl_pix_fmt == SDL_PIXELFORMAT_UNKNOWN ? SDL_PIXELFORMAT_ARGB8888 : sdl_pix_fmt, frame->width, frame->height, sdl_blendmode, false))
        return false;
    switch (sdl_pix_fmt) {
    case SDL_PIXELFORMAT_UNKNOWN:
        /* This should only happen if we are not using avfilter... */
        *img_convert_ctx = sws_getCachedContext(*img_convert_ctx,
            frame->width, frame->height, static_cast<AVPixelFormat>(frame->format), frame->width, frame->height,
            AV_PIX_FMT_BGRA, sws_flags, nullptr, nullptr, nullptr);
        if (*img_convert_ctx != nullptr) {
            uint8_t* pixels[4];
            int pitch[4];
            if (!SDL_LockTexture(*tex, nullptr, (void**)pixels, pitch)) {
                sws_scale(*img_convert_ctx, (const uint8_t* const*)frame->data, frame->linesize, 0, frame->height, pixels, pitch);
                SDL_UnlockTexture(*tex);
            }
        }
        else {
            av_log(nullptr, AV_LOG_FATAL, "Cannot initialize the conversion context\n");
            ret = -1;
        }
        break;
    case SDL_PIXELFORMAT_IYUV:
        if (frame->linesize[0] > 0 && frame->linesize[1] > 0 && frame->linesize[2] > 0) {
            ret = SDL_UpdateYUVTexture(*tex, nullptr,   frame->data[0], frame->linesize[0],
                                                        frame->data[1], frame->linesize[1],
                                                        frame->data[2], frame->linesize[2]);
        }
        else if (frame->linesize[0] < 0 && frame->linesize[1] < 0 && frame->linesize[2] < 0) {
            ret = SDL_UpdateYUVTexture(*tex, nullptr,   frame->data[0] + frame->linesize[0] * (frame->height - 1), -frame->linesize[0],
                                                        frame->data[1] + frame->linesize[1] * (AV_CEIL_RSHIFT(frame->height, 1) - 1), -frame->linesize[1],
                                                        frame->data[2] + frame->linesize[2] * (AV_CEIL_RSHIFT(frame->height, 1) - 1), -frame->linesize[2]);
        }
        else {
            av_log(nullptr, AV_LOG_ERROR, "Mixed negative and positive linesizes are not supported.\n");
            return false;
        }
        break;
    default:
        if (frame->linesize[0] < 0)
            ret = SDL_UpdateTexture(*tex, nullptr, frame->data[0] + frame->linesize[0] * (frame->height - 1), -frame->linesize[0]);
        else
            ret = SDL_UpdateTexture(*tex, nullptr, frame->data[0], frame->linesize[0]);
        break;
    }
    return (ret == 0) ? true : false;
}

void FMediaPlayer::set_sdl_yuv_conversion_mode(AVFrame* frame)
{
#if SDL_VERSION_ATLEAST(2,0,8)
    SDL_YUV_CONVERSION_MODE mode = SDL_YUV_CONVERSION_AUTOMATIC;
    if (frame && (frame->format == AV_PIX_FMT_YUV420P || frame->format == AV_PIX_FMT_YUYV422 || frame->format == AV_PIX_FMT_UYVY422)) {
        if (frame->color_range == AVCOL_RANGE_JPEG)
            mode = SDL_YUV_CONVERSION_JPEG;
        else if (frame->colorspace == AVCOL_SPC_BT709)
            mode = SDL_YUV_CONVERSION_BT709;
        else if (frame->colorspace == AVCOL_SPC_BT470BG || frame->colorspace == AVCOL_SPC_SMPTE170M || frame->colorspace == AVCOL_SPC_SMPTE240M)
            mode = SDL_YUV_CONVERSION_BT601;
    }
    SDL_SetYUVConversionMode(mode);
#endif
}

void FMediaPlayer::video_image_display()
{
    SFrame* vp = nullptr;
    SFrame* sp = nullptr;
    SDL_Rect rect{0, 0, 0, 0};

    vp = this->pictq.PeekLast();
    /* ???????????¦Λ????? */
    if (this->pSubtitleStream && (this->subpq.NbRemaining() > 0))
    {
        sp = this->subpq.Peek();

        if (vp->pts >= sp->pts + ((float)sp->sub.start_display_time / 1000)) {
            if (!sp->uploaded) {
                uint8_t* pixels[4];
                int pitch[4];

                if (!sp->width || !sp->height) {
                    sp->width = vp->width;
                    sp->height = vp->height;
                }
                if (!realloc_texture(&this->sub_texture, SDL_PIXELFORMAT_ARGB8888, sp->width, sp->height, SDL_BLENDMODE_BLEND, true))
                    return;

                for (int i = 0; i < sp->sub.num_rects; i++)
                {
                    AVSubtitleRect* sub_rect = sp->sub.rects[i];

                    sub_rect->x = av_clip(sub_rect->x, 0, sp->width);
                    sub_rect->y = av_clip(sub_rect->y, 0, sp->height);
                    sub_rect->w = av_clip(sub_rect->w, 0, sp->width - sub_rect->x);
                    sub_rect->h = av_clip(sub_rect->h, 0, sp->height - sub_rect->y);

                    this->sub_convert_ctx = sws_getCachedContext(this->sub_convert_ctx,
                        sub_rect->w, sub_rect->h, AV_PIX_FMT_PAL8,
                        sub_rect->w, sub_rect->h, AV_PIX_FMT_BGRA,
                        0, nullptr, nullptr, nullptr);
                    if (!this->sub_convert_ctx) {
                        av_log(nullptr, AV_LOG_FATAL, "Cannot initialize the conversion context\n");
                        return;
                    }
                    if (!SDL_LockTexture(this->sub_texture, (SDL_Rect*)sub_rect, (void**)pixels, pitch)) {
                        sws_scale(this->sub_convert_ctx, (const uint8_t* const*)sub_rect->data, sub_rect->linesize,
                            0, sub_rect->h, pixels, pitch);
                        SDL_UnlockTexture(this->sub_texture);
                    }
                }
                sp->uploaded = true;
            }
        }
        else sp = nullptr;
    }

    calculate_display_rect(rect, this->rect, vp->width, vp->height, vp->sar);

    if (!vp->uploaded)
    {
        if (!upload_texture(&this->vid_texture, vp->frame, &this->img_convert_ctx))
            return;
        vp->uploaded = true;
        vp->flip_v = vp->frame->linesize[0] < 0;
    }

    /* ??????????? */
    set_sdl_yuv_conversion_mode(vp->frame);
    SDL_RenderCopyEx(pRenderer, this->vid_texture, nullptr, &rect, 0, nullptr, static_cast<SDL_RendererFlip>(vp->flip_v ? SDL_FLIP_VERTICAL : 0));
    set_sdl_yuv_conversion_mode(nullptr);

    /* ??????????? */
    if (sp) {
#if USE_ONEPASS_SUBTITLE_RENDER
        SDL_RenderCopy(renderer, this->sub_texture, nullptr, &rect);
#else

        const double xratio = (double)rect.w / (double)sp->width;
        const double yratio = (double)rect.h / (double)sp->height;
        for (int i = 0; i < sp->sub.num_rects; i++)
        {
            const SDL_Rect* sub_rect = (SDL_Rect*)sp->sub.rects[i];
            SDL_Rect target;
            target.x = int(rect.x + sub_rect->x * xratio);
            target.y = int(rect.y + sub_rect->y * yratio);
            target.w = int(sub_rect->w * xratio);
            target.h = int(sub_rect->h * yratio);
            SDL_RenderCopy(pRenderer, this->sub_texture, sub_rect, &target);
        }
#endif
    }
}

int FMediaPlayer::compute_mod(int a, int b)
{
    return a < 0 ? a % b + b : a % b;
}

void FMediaPlayer::video_audio_display()
{
    int i, i_start, x, y1, y, ys, delay, n, nb_display_channels;
    int channels, h, h2;
    int64_t time_diff;
    int rdft_bits, nb_freq;

    for (rdft_bits = 1; (1 << rdft_bits) < 2 * this->rect.h; rdft_bits++)
        ;
    nb_freq = 1 << (rdft_bits - 1);

    /* compute display index : center on currently output samples */
    channels = this->audio_tgt.channels;
    nb_display_channels = channels;
    if (!this->paused)
    {
        int data_used = this->eShow_mode == FMediaPlayer::EShowMode::SHOW_MODE_WAVES ? this->rect.w : (2 * nb_freq);
        n = 2 * channels;
        delay = this->audio_write_buf_size;
        delay /= n;

        /* to be more precise, we take into account the time spent since
           the last buffer computation */
        if (audio_callback_time) {
            time_diff = av_gettime_relative() - audio_callback_time;
            delay -= (time_diff * this->audio_tgt.freq) / 1000000;
        }

        delay += 2 * data_used;
        if (delay < data_used)
            delay = data_used;

        i_start = x = compute_mod(this->sample_array_index - delay * channels, SAMPLE_ARRAY_SIZE);
        if (this->eShow_mode == FMediaPlayer::EShowMode::SHOW_MODE_WAVES)
        {
            h = INT_MIN;
            for (i = 0; i < 1000; i += channels) {
                int idx = (SAMPLE_ARRAY_SIZE + x - i) % SAMPLE_ARRAY_SIZE;
                int a = this->sample_array[idx];
                int b = this->sample_array[(idx + 4 * channels) % SAMPLE_ARRAY_SIZE];
                int c = this->sample_array[(idx + 5 * channels) % SAMPLE_ARRAY_SIZE];
                int d = this->sample_array[(idx + 9 * channels) % SAMPLE_ARRAY_SIZE];
                int score = a - d;
                if (h < score && (b ^ c) < 0) {
                    h = score;
                    i_start = idx;
                }
            }
        }

        this->last_i_start = i_start;
    }
    else i_start = this->last_i_start;

    if (this->eShow_mode == FMediaPlayer::EShowMode::SHOW_MODE_WAVES)
    {
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

        /* total height for one channel */
        h = this->rect.h / nb_display_channels;
        /* graph height / 2 */
        h2 = (h * 9) / 20;
        for (int ch = 0; ch < nb_display_channels; ch++)
        {
            i = i_start + ch;
            y1 = this->rect.y + ch * h + (h / 2); /* position of center line */
            for (x = 0; x < this->rect.w; x++) {
                y = (this->sample_array[i] * h2) >> 15;
                if (y < 0) {
                    y = -y;
                    ys = y1 - y;
                }
                else {
                    ys = y1;
                }
                fill_rectangle(this->rect.x + x, ys, 1, y);
                i += channels;
                if (i >= SAMPLE_ARRAY_SIZE)
                    i -= SAMPLE_ARRAY_SIZE;
            }
        }

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 255, 255);

        for (int ch = 1; ch < nb_display_channels; ch++) {
            y = this->rect.y + ch * h;
            fill_rectangle(this->rect.x, y, this->rect.w, 1);
        }
    }
    else {
        if (!realloc_texture(&this->vis_texture, SDL_PIXELFORMAT_ARGB8888, this->rect.w, this->rect.h, SDL_BLENDMODE_NONE, true))
            return;

        nb_display_channels = FFMIN(nb_display_channels, 2);
        if (rdft_bits != this->rdft_bits) {
            av_rdft_end(this->rdft);
            av_free(this->rdft_data);
            this->rdft = av_rdft_init(rdft_bits, DFT_R2C);
            this->rdft_bits = rdft_bits;
            this->rdft_data = static_cast<FFTSample*>(av_malloc_array(nb_freq, 4 * sizeof(*this->rdft_data)));
        }
        if (!this->rdft || !this->rdft_data) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to allocate buffers for RDFT, switching to waves display\n");
            this->eShow_mode = FMediaPlayer::EShowMode::SHOW_MODE_WAVES;
        }
        else {
            FFTSample* data[2];
            SDL_Rect rect;
            rect.x = this->xpos; rect.y = 0; rect.w = 1; rect.h = this->rect.h;

            uint32_t* pixels = nullptr;
            int pitch;
            for (int ch = 0; ch < nb_display_channels; ch++) {
                data[ch] = this->rdft_data + 2 * nb_freq * ch;
                i = i_start + ch;
                for (x = 0; x < 2 * nb_freq; x++) {
                    double w = (x - nb_freq) * (1.0 / nb_freq);
                    data[ch][x] = this->sample_array[i] * (1.0 - w * w);
                    i += channels;
                    if (i >= SAMPLE_ARRAY_SIZE)
                        i -= SAMPLE_ARRAY_SIZE;
                }
                av_rdft_calc(this->rdft, data[ch]);
            }
            /* Least efficient way to do this, we should of course
             * directly access it but it is more than fast enough. */
            if (!SDL_LockTexture(this->vis_texture, &rect, (void**)&pixels, &pitch))
            {
                pitch >>= 2;
                pixels += pitch * this->rect.h;
                for (y = 0; y < this->rect.h; y++) {
                    double w = 1 / sqrt(nb_freq);
                    int a = sqrt(w * sqrt(data[0][2 * y + 0] * data[0][2 * y + 0] + data[0][2 * y + 1] * data[0][2 * y + 1]));
                    int b = (nb_display_channels == 2) ? sqrt(w * hypot(data[1][2 * y + 0], data[1][2 * y + 1]))
                        : a;
                    a = FFMIN(a, 255);
                    b = FFMIN(b, 255);
                    pixels -= pitch;
                    *pixels = (a << 16) + (b << 8) + ((a + b) >> 1);
                }
                SDL_UnlockTexture(this->vis_texture);
            }
            SDL_RenderCopy(pRenderer, this->vis_texture, nullptr, nullptr);
        }
        if (!this->paused)
            this->xpos++;
        if (this->xpos >= this->rect.w)
            this->xpos = this->rect.x;
    }
}

void FMediaPlayer::stream_component_close(int nStreamIndex)
{
    AVCodecParameters* codecpar = nullptr;

    if (nStreamIndex < 0 || nStreamIndex >= this->pFormatCtx->nb_streams)
        return;
    codecpar = this->pFormatCtx->streams[nStreamIndex]->codecpar;

    switch (codecpar->codec_type)
    {
    case AVMEDIA_TYPE_AUDIO:
        this->auddec.Abort(&this->sampq);
        SDL_CloseAudioDevice(audio_dev);
        this->auddec.Destroy();
        swr_free(&this->swr_ctx);
        av_freep(&this->audio_buf1);
        this->audio_buf1_size = 0;
        this->audio_buf = nullptr;

        if (this->rdft) {
            av_rdft_end(this->rdft);
            av_freep(&this->rdft_data);
            this->rdft = nullptr;
            this->rdft_bits = 0;
        }
        break;
    case AVMEDIA_TYPE_VIDEO:
        this->viddec.Abort(&this->pictq);
        this->viddec.Destroy();
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        this->subdec.Abort(&this->subpq);
        this->subdec.Destroy();
        break;
    default:
        break;
    }

    this->pFormatCtx->streams[nStreamIndex]->discard = AVDISCARD_ALL;
    switch (codecpar->codec_type)
    {
    case AVMEDIA_TYPE_AUDIO:
        this->pAudioStream = nullptr;
        this->audio_stream = -1;
        break;
    case AVMEDIA_TYPE_VIDEO:
        this->pVideoStream = nullptr;
        this->video_stream = -1;
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        this->pSubtitleStream = nullptr;
        this->subtitle_stream = -1;
        break;
    default:
        break;
    }
}

void FMediaPlayer::StreamClose()
{
    /* ??????????? */
    this->abort_request = true;

    /* ??????????? */
    if (this->future.valid())
        if (this->future.get() != 0)
            av_log(nullptr, AV_LOG_WARNING, "Thread exit exception\n");

    /* ???¦Ι?????????? */
    if (this->audio_stream >= 0)
    {
        stream_component_close(this->audio_stream);
        audio_stream = -1;
    }
    if (this->video_stream >= 0)
    {
        stream_component_close(this->video_stream);
        video_stream = -1;
    }
    if (this->subtitle_stream >= 0)
    {
        stream_component_close(this->subtitle_stream);
        subtitle_stream = -1;
    }

    avformat_close_input(&this->pFormatCtx);
    this->pFormatCtx = nullptr;

    this->videoq.Destroy();
    this->audioq.Destroy();
    this->subtitleq.Destroy();

    /* free all pictures */
    this->pictq.Destory();
    this->sampq.Destory();
    this->subpq.Destory();
    sws_freeContext(this->img_convert_ctx);
    this->img_convert_ctx = nullptr;
    sws_freeContext(this->sub_convert_ctx);
    this->sub_convert_ctx = nullptr;
    this->sURL = "";
}

void FMediaPlayer::OnExit()
{
    StreamClose();

    uninitRender();

#if CONFIG_AVFILTER
    av_freep(&vfilters_list);
#endif

    av_log(nullptr, AV_LOG_QUIET, "Exit Now\n");

    return;
}

void FMediaPlayer::sigterm_handler(int sig)
{
    exit(123);
}

bool FMediaPlayer::video_open()
{
    const int w = screen_width ? screen_width : default_width;
    const int h = screen_height ? screen_height : default_height;

#if defined(SDL_MAIN_HANDLED)
    SDL_SetWindowTitle(pWindow, "Ubuntu");
#endif

    SDL_SetWindowSize(pWindow, w, h);
    SDL_SetWindowPosition(pWindow, screen_left, screen_top);
    if (is_full_screen)
        if (SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) == -1)
            return false;
    SDL_ShowWindow(pWindow);

    this->rect.w = w;
    this->rect.h = h;

    return true;
}

void FMediaPlayer::video_display()
{
    try
    {
        /* @TODO */
        //if (!this->rect.w)
//        static bool initFlag = false;
//        if(!initFlag && this->rect.w)
//        {
            video_open();
//            initFlag = true;
//        }

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);
        if (this->pAudioStream && this->eShow_mode != FMediaPlayer::EShowMode::SHOW_MODE_VIDEO)
            video_audio_display();
        else if (this->pVideoStream)
            video_image_display();
        SDL_RenderPresent(pRenderer);
    }
    catch (...)
    {
        av_log(nullptr, AV_LOG_ERROR, "Error::video_display\n");
        return;
    }
}

void FMediaPlayer::sync_clock_to_slave(SClock& c, SClock& slave)
{
    const double clock = c.Get();
    const double slave_clock = slave.Get();
    if ( !std::isnan(slave_clock) &&
         (std::isnan(clock) || (std::fabs(clock - slave_clock) > AV_NOSYNC_THRESHOLD) )
       )
        c.Set(slave_clock, slave.serial);
}

FMediaPlayer::ESyncType FMediaPlayer::get_master_sync_type()
{
    if (this->av_sync_type == ESyncType::AV_SYNC_VIDEO_MASTER)
    {
        if (this->pVideoStream)
            return ESyncType::AV_SYNC_VIDEO_MASTER;
        else
            return ESyncType::AV_SYNC_AUDIO_MASTER;
    }
    else if (this->av_sync_type == ESyncType::AV_SYNC_AUDIO_MASTER)
    {
        if (this->pAudioStream)
            return ESyncType::AV_SYNC_AUDIO_MASTER;
        else
            return ESyncType::AV_SYNC_EXTERNAL_CLOCK;
    }
    else {
        return ESyncType::AV_SYNC_EXTERNAL_CLOCK;
    }
}

double FMediaPlayer::get_master_clock()
{
    double val = 0.0;

    switch (get_master_sync_type())
    {
    case ESyncType::AV_SYNC_VIDEO_MASTER:
        val = this->vidclk.Get();
        break;
    case ESyncType::AV_SYNC_AUDIO_MASTER:
        val = this->audclk.Get();
        break;
    default:
        val = this->extclk.Get();
        break;
    }
    return val;
}

void FMediaPlayer::check_external_clock_speed()
{
    if (this->video_stream >= 0 && this->videoq.nb_packets <= EXTERNAL_CLOCK_MIN_FRAMES ||
        this->audio_stream >= 0 && this->audioq.nb_packets <= EXTERNAL_CLOCK_MIN_FRAMES)
        this->extclk.SetSpeed(FFMAX(EXTERNAL_CLOCK_SPEED_MIN, this->extclk.speed - EXTERNAL_CLOCK_SPEED_STEP));
    else if ((this->video_stream < 0 || this->videoq.nb_packets > EXTERNAL_CLOCK_MAX_FRAMES) &&
        (this->audio_stream < 0 || this->audioq.nb_packets > EXTERNAL_CLOCK_MAX_FRAMES))
        this->extclk.SetSpeed(FFMIN(EXTERNAL_CLOCK_SPEED_MAX, this->extclk.speed + EXTERNAL_CLOCK_SPEED_STEP));
    else {
        double speed = this->extclk.speed;
        if (speed != 1.0)
            this->extclk.SetSpeed(speed + EXTERNAL_CLOCK_SPEED_STEP * (1.0 - speed) / fabs(1.0 - speed));
    }
}

void FMediaPlayer::OnStreamSeek(int64_t pos, int64_t rel, bool seek_by_bytes)
{
    if (!this->seek_req)
    {
        this->seek_pos = pos;
        this->seek_rel = rel;
        this->seek_flags &= ~AVSEEK_FLAG_BYTE;
        if (seek_by_bytes)
            this->seek_flags |= AVSEEK_FLAG_BYTE;
        this->seek_req = true;
        this->continue_read_thread->notify_one();
    }
}

void FMediaPlayer::stream_toggle_pause()
{
    if (this->paused)
    {
        this->frame_timer += av_gettime_relative() / 1000000.0 - this->vidclk.last_updated;
        if (this->read_pause_return != AVERROR(ENOSYS))
            this->vidclk.paused = false;

        this->vidclk.Set(this->vidclk.Get(), this->vidclk.serial);
    }
    this->extclk.Set(this->extclk.Get(), this->extclk.serial);
    this->paused = this->audclk.paused = this->vidclk.paused = this->extclk.paused = !this->paused;
}

void FMediaPlayer::OnTogglePause()
{
    stream_toggle_pause();
    this->step = false;
}

void FMediaPlayer::OnToggleMute()
{
    this->muted = !this->muted;
}

void FMediaPlayer::OnUpdateVolume(int sign, double step)
{
    double volume_level = this->audio_volume ? (20 * log(this->audio_volume / (double)SDL_MIX_MAXVOLUME) / log(10)) : -1000.0;
    int new_volume = lrint(SDL_MIX_MAXVOLUME * pow(10.0, (volume_level + sign * step) / 20.0));
    this->audio_volume = av_clip(this->audio_volume == new_volume ? (this->audio_volume + sign) : new_volume, 0, SDL_MIX_MAXVOLUME);
}

void FMediaPlayer::OnStepToNextFrame()
{
    /* if the stream is paused unpause it, then step */
    if (this->paused)
        stream_toggle_pause();
    this->step = true;
}

double FMediaPlayer::compute_target_delay(double delay)
{
    double sync_threshold = 0, diff = 0;

    /* update delay to follow master synchronisation source */
    if (get_master_sync_type() != ESyncType::AV_SYNC_VIDEO_MASTER)
    {
        /* if video is slave, we try to correct big delays by duplicating or deleting a frame */
        diff = this->vidclk.Get() - get_master_clock();

        /* skip or repeat frame. We take into account the
           delay to compute the threshold. I still don't know
           if it is the best guess */
        sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, delay));
        if (!std::isnan(diff) && std::fabs(diff) < this->max_frame_duration)
        {
            if (diff <= -sync_threshold)
                delay = FFMAX(0, delay + diff);
            else if (diff >= sync_threshold && delay > AV_SYNC_FRAMEDUP_THRESHOLD)
                delay = delay + diff;
            else if (diff >= sync_threshold)
                delay = 2 * delay;
        }
    }

    av_log(nullptr, AV_LOG_TRACE, "video: delay=%0.3f A-V=%f\n", delay, -diff);

    return delay;
}

double FMediaPlayer::vp_duration(SFrame* vp, SFrame* nextvp)
{
    if (vp->serial == nextvp->serial)
    {
        double duration = nextvp->pts - vp->pts;
        if (std::isnan(duration) || duration <= 0 || duration > this->max_frame_duration)
            return vp->duration;
        else
            return duration;
    }
    else return 0.0;
}

void FMediaPlayer::update_video_pts(double pts, int64_t pos, int serial)
{
    /* update current video pts */
    this->vidclk.Set(pts, serial);
    sync_clock_to_slave(this->extclk, this->vidclk);
}

void FMediaPlayer::video_refresh(double& remaining_time)
{
    double time = 0.0;

    SFrame* sp = nullptr, * sp2 = nullptr;

    if (!this->paused && get_master_sync_type() == FMediaPlayer::ESyncType::AV_SYNC_EXTERNAL_CLOCK && this->is_realtime())
        check_external_clock_speed();

    if (this->eShow_mode != FMediaPlayer::EShowMode::SHOW_MODE_VIDEO && this->pAudioStream)
    {
        time = av_gettime_relative() / 1000000.0;
        if (this->force_refresh || this->last_vis_time + rdftspeed < time)
        {
            video_display();
            this->last_vis_time = time;
        }
        remaining_time = FFMIN(remaining_time, this->last_vis_time + rdftspeed - time);
    }

    if (this->pVideoStream) {
    retry:
        if (this->pictq.NbRemaining() == 0) {
            // nothing to do, no picture to display in the queue
        }
        else {
            double duration, delay;
            SFrame* vp = nullptr, * lastvp = nullptr;

            /* dequeue the picture */
            lastvp = this->pictq.PeekLast();
            vp = this->pictq.Peek();

            if (vp->serial != this->videoq.serial) {
                this->pictq.Next();
                goto retry;
            }

            if (lastvp->serial != vp->serial)
                this->frame_timer = av_gettime_relative() / 1000000.0;

            if (this->paused)
                goto display;

            /* compute nominal last_duration */
            delay = compute_target_delay(vp_duration(lastvp, vp));

            time = av_gettime_relative() / 1000000.0;
            if (time < this->frame_timer + delay) {
                remaining_time = FFMIN(this->frame_timer + delay - time, remaining_time);
                goto display;
            }

            this->frame_timer += delay;
            if (delay > 0 && time - this->frame_timer > AV_SYNC_THRESHOLD_MAX)
                this->frame_timer = time;

            {
                std::lock_guard<std::mutex> lock(this->pictq.mutex);
                if (!std::isnan(vp->pts))
                    update_video_pts(vp->pts, vp->pos, vp->serial);
            }

            if (this->pictq.NbRemaining() > 1) {
                SFrame* nextvp = this->pictq.PeekNext();
                duration = vp_duration(vp, nextvp);
                if (!this->step && (framedrop || (framedrop && get_master_sync_type() != FMediaPlayer::ESyncType::AV_SYNC_VIDEO_MASTER)) && time > this->frame_timer + duration) {
                    this->frame_drops_late++;
                    this->pictq.Next();
                    goto retry;
                }
            }

            if (this->pSubtitleStream) {
                while (this->subpq.NbRemaining() > 0) {
                    sp = this->subpq.Peek();

                    if (this->subpq.NbRemaining() > 1)
                        sp2 = this->subpq.PeekNext();
                    else
                        sp2 = nullptr;

                    if (sp->serial != this->subtitleq.serial
                        || (this->vidclk.pts > (sp->pts + ((float)sp->sub.end_display_time / 1000)))
                        || (sp2 && this->vidclk.pts > (sp2->pts + ((float)sp2->sub.start_display_time / 1000))))
                    {
                        if (sp->uploaded) {
                            for (int i = 0; i < sp->sub.num_rects; i++) {
                                AVSubtitleRect* sub_rect = sp->sub.rects[i];
                                uint8_t* pixels = nullptr;
                                int pitch, j;

                                if (!SDL_LockTexture(this->sub_texture, (SDL_Rect*)sub_rect, (void**)&pixels, &pitch)) {
                                    for (j = 0; j < sub_rect->h; j++, pixels += pitch)
                                        memset(pixels, 0, sub_rect->w << 2);
                                    SDL_UnlockTexture(this->sub_texture);
                                }
                            }
                        }
                        this->subpq.Next();
                    }
                    else {
                        break;
                    }
                }
            }

            this->pictq.Next();
            this->force_refresh = true;

            if (this->step && !this->paused)
                stream_toggle_pause();
        }
    display:
        /* display picture */
        if (this->force_refresh && this->eShow_mode == FMediaPlayer::EShowMode::SHOW_MODE_VIDEO && this->pictq.rindex_shown)
            video_display();
    }
    this->force_refresh = false;
    if (/*show_status*/true) {
        static int64_t last_time;
        int64_t cur_time;
        int aqsize, vqsize, sqsize;
        double av_diff;

        cur_time = av_gettime_relative();
        if (!last_time || (cur_time - last_time) >= 30000)
        {
            aqsize = vqsize = sqsize = 0;
            if (this->pAudioStream)
                aqsize = this->audioq.size;
            if (this->pVideoStream)
                vqsize = this->videoq.size;
            if (this->pSubtitleStream)
                sqsize = this->subtitleq.size;
            av_diff = 0;
            if (this->pAudioStream && this->pVideoStream)
                av_diff = this->audclk.Get() - this->vidclk.Get();
            else if (this->pVideoStream)
                av_diff = get_master_clock() - this->vidclk.Get();
            else if (this->pAudioStream)
                av_diff = get_master_clock() - this->audclk.Get();
            /*av_log(nullptr, AV_LOG_INFO,
                "%7.2f %s:%7.3f fd=%4d aq=%5dKB vq=%5dKB sq=%5dB f=%"PRId64"/%"PRId64"   \r",
                get_master_clock(is),
                (pPlayer->audio_st && pPlayer->video_st) ? "A-V" : (pPlayer->video_st ? "M-V" : (pPlayer->audio_st ? "M-A" : "   ")),
                av_diff,
                pPlayer->frame_drops_early + pPlayer->frame_drops_late,
                aqsize / 1024,
                vqsize / 1024,
                sqsize,
                pPlayer->video_st ? pPlayer->viddec.avctx->pts_correction_num_faulty_dts : 0,
                pPlayer->video_st ? pPlayer->viddec.avctx->pts_correction_num_faulty_pts : 0);*/
            fflush(stdout);
            last_time = cur_time;
        }
    }
}

int FMediaPlayer::queue_picture(AVFrame* src_frame, double pts, double duration, int64_t pos, int serial)
{
    SFrame* vp = nullptr;

#if defined(DEBUG_SYNC)
    printf("frame_type=%c pts=%0.3f\n",
        av_get_picture_type_char(src_frame->pict_type), pts);
#endif

    if (!(vp = this->pictq.PeekWritable()))
        return -1;

    vp->sar = src_frame->sample_aspect_ratio;
    vp->uploaded = false;

    vp->width = src_frame->width;
    vp->height = src_frame->height;
    vp->format = src_frame->format;

    vp->pts = pts;
    vp->duration = duration;
    vp->pos = pos;
    vp->serial = serial;

    OnWindowSizeChange(vp->width, vp->height, vp->sar);

    av_frame_move_ref(vp->frame, src_frame);
    this->pictq.Push();
    return 0;
}

int FMediaPlayer::get_video_frame(AVFrame* frame)
{
    int got_picture = -1;

    if ((got_picture = this->viddec.Decode(frame, nullptr)) < 0)
        return -1;

    if (got_picture)
    {
        double dpts{double(NAN)};

        if (frame->pts != AV_NOPTS_VALUE)
            dpts = av_q2d(this->pVideoStream->time_base) * frame->pts;

        frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(this->pFormatCtx, this->pVideoStream, frame);

        if (framedrop || (framedrop && get_master_sync_type() != ESyncType::AV_SYNC_VIDEO_MASTER))
        {
            if (frame->pts != AV_NOPTS_VALUE)
            {
                double diff = dpts - get_master_clock();
                if (!std::isnan(diff) &&
                    (std::fabs(diff) < AV_NOSYNC_THRESHOLD) &&
                    (diff - this->frame_last_filter_delay < 0) &&
                    (this->viddec.pkt_serial == this->vidclk.serial) &&
                    this->videoq.nb_packets
                   )
                {
                    this->frame_drops_early++;
                    av_frame_unref(frame);
                    got_picture = 0;
                }
            }
        }
    }

    return got_picture;
}

#if CONFIG_AVFILTER
int64_t FMediaPlayer::get_valid_channel_layout(int64_t channel_layout, int channels)
{
    if (channel_layout && av_get_channel_layout_nb_channels(channel_layout) == channels)
        return channel_layout;
    else
        return 0;
}

int FMediaPlayer::cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
    enum AVSampleFormat fmt2, int64_t channel_count2)
{
    /* If channel count == 1, planar and non-planar formats are the same */
    if (channel_count1 == 1 && channel_count2 == 1)
        return av_get_packed_sample_fmt(fmt1) != av_get_packed_sample_fmt(fmt2);
    else
        return channel_count1 != channel_count2 || fmt1 != fmt2;
}

double FMediaPlayer::get_rotation(AVStream* st)
{
    uint8_t* displaymatrix = av_stream_get_side_data(st, AV_PKT_DATA_DISPLAYMATRIX, NULL);
    double theta = 0;
    if (displaymatrix)
        theta = -av_display_rotation_get((int32_t*)displaymatrix);

    theta -= 360 * floor(theta / 360 + 0.9 / 360);

    if (fabs(theta - 90 * round(theta / 90)) > 2)
        av_log(NULL, AV_LOG_WARNING, "Odd rotation angle.\n"
            "If you want to help, upload a sample "
            "of this file to ftp://upload.ffmpeg.org/incoming/ "
            "and contact the ffmpeg-devel mailing list. (ffmpeg-devel@ffmpeg.org)");

    return theta;
}

int FMediaPlayer::configure_filtergraph(AVFilterGraph* graph, const char* filtergraph,
    AVFilterContext* source_ctx, AVFilterContext* sink_ctx)
{
    int ret, i;
    int nb_filters = graph->nb_filters;
    AVFilterInOut* outputs = nullptr, * inputs = nullptr;

    if (filtergraph) {
        outputs = avfilter_inout_alloc();
        inputs = avfilter_inout_alloc();
        if (!outputs || !inputs) {
            ret = AVERROR(ENOMEM);
            goto fail;
        }

        outputs->name = av_strdup("in");
        outputs->filter_ctx = source_ctx;
        outputs->pad_idx = 0;
        outputs->next = nullptr;

        inputs->name = av_strdup("out");
        inputs->filter_ctx = sink_ctx;
        inputs->pad_idx = 0;
        inputs->next = nullptr;

        if ((ret = avfilter_graph_parse_ptr(graph, filtergraph, &inputs, &outputs, nullptr)) < 0)
            goto fail;
    }
    else {
        if ((ret = avfilter_link(source_ctx, 0, sink_ctx, 0)) < 0)
            goto fail;
    }

    /* Reorder the filters to ensure that inputs of the custom filters are merged first */
    for (i = 0; i < graph->nb_filters - nb_filters; i++)
        FFSWAP(AVFilterContext*, graph->filters[i], graph->filters[i + nb_filters]);

    ret = avfilter_graph_config(graph, nullptr);
fail:
    avfilter_inout_free(&outputs);
    avfilter_inout_free(&inputs);
    return ret;
}

int FMediaPlayer::configure_video_filters(AVFilterGraph* graph, const char* vfilters, AVFrame* frame)
{
    enum AVPixelFormat pix_fmts[FF_ARRAY_ELEMS(sdl_texture_format_map)];
    char sws_flags_str[512] = "";
    char buffersrc_args[256];
    int ret;
    AVFilterContext* filt_src = nullptr, * filt_out = nullptr, * last_filter = nullptr;
    AVCodecParameters* codecpar = this->pVideoStream->codecpar;
    AVRational fr = av_guess_frame_rate(this->pFormatCtx, this->pVideoStream, nullptr);
    AVDictionaryEntry* e = nullptr;
    int nb_pix_fmts = 0;
    int i, j;

    for (i = 0; i < renderer_info.num_texture_formats; i++) {
        for (j = 0; j < FF_ARRAY_ELEMS(sdl_texture_format_map) - 1; j++) {
            if (renderer_info.texture_formats[i] == sdl_texture_format_map[j].texture_fmt) {
                pix_fmts[nb_pix_fmts++] = sdl_texture_format_map[j].format;
                break;
            }
        }
    }
    pix_fmts[nb_pix_fmts] = AV_PIX_FMT_NONE;

    while ((e = av_dict_get(sws_dict, "", e, AV_DICT_IGNORE_SUFFIX))) {
        if (!strcmp(e->key, "sws_flags")) {
            av_strlcatf(sws_flags_str, sizeof(sws_flags_str), "%s=%s:", "flags", e->value);
        }
        else
            av_strlcatf(sws_flags_str, sizeof(sws_flags_str), "%s=%s:", e->key, e->value);
    }
    if (strlen(sws_flags_str))
        sws_flags_str[strlen(sws_flags_str) - 1] = '\0';

    graph->scale_sws_opts = av_strdup(sws_flags_str);

    snprintf(buffersrc_args, sizeof(buffersrc_args),
        "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
        frame->width, frame->height, frame->format,
        this->pVideoStream->time_base.num, this->pVideoStream->time_base.den,
        codecpar->sample_aspect_ratio.num, FFMAX(codecpar->sample_aspect_ratio.den, 1));
    if (fr.num && fr.den)
        av_strlcatf(buffersrc_args, sizeof(buffersrc_args), ":frame_rate=%d/%d", fr.num, fr.den);

    if ((ret = avfilter_graph_create_filter(&filt_src,
        avfilter_get_by_name("buffer"),
        "ffplay_buffer", buffersrc_args, nullptr,
        graph)) < 0)
        goto fail;

    ret = avfilter_graph_create_filter(&filt_out,
        avfilter_get_by_name("buffersink"),
        "ffplay_buffersink", nullptr, nullptr, graph);
    if (ret < 0)
        goto fail;

    if ((ret = av_opt_set_int_list(filt_out, "pix_fmts", pix_fmts, AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto fail;

    last_filter = filt_out;

    /* Note: this macro adds a filter before the lastly added filter, so the
     * processing order of the filters is in reverse */
#define INSERT_FILT(name, arg) do {                                          \
    AVFilterContext *filt_ctx;                                               \
                                                                             \
    ret = avfilter_graph_create_filter(&filt_ctx,                            \
                                       avfilter_get_by_name(name),           \
                                       "ffplay_" name, arg, nullptr, graph); \
    if (ret < 0)                                                             \
        goto fail;                                                           \
                                                                             \
    ret = avfilter_link(filt_ctx, 0, last_filter, 0);                        \
    if (ret < 0)                                                             \
        goto fail;                                                           \
                                                                             \
    last_filter = filt_ctx;                                                  \
} while (0)

    if (autorotate) {
        double theta = get_rotation(this->pVideoStream);

        if (fabs(theta - 90) < 1.0) {
            INSERT_FILT("transpose", "clock");
        }
        else if (fabs(theta - 180) < 1.0) {
            INSERT_FILT("hflip", nullptr);
            INSERT_FILT("vflip", nullptr);
        }
        else if (fabs(theta - 270) < 1.0) {
            INSERT_FILT("transpose", "cclock");
        }
        else if (fabs(theta) > 1.0) {
            char rotate_buf[64];
            snprintf(rotate_buf, sizeof(rotate_buf), "%f*PI/180", theta);
            INSERT_FILT("rotate", rotate_buf);
        }
    }

    if ((ret = configure_filtergraph(graph, vfilters, filt_src, last_filter)) < 0)
        goto fail;

    this->in_video_filter = filt_src;
    this->out_video_filter = filt_out;

fail:
    return ret;
}

int FMediaPlayer::configure_audio_filters(const char* afilters, int force_output_format)
{
    static const enum AVSampleFormat sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
    int sample_rates[2] = { 0, -1 };
    int64_t channel_layouts[2] = { 0, -1 };
    int channels[2] = { 0, -1 };
    AVFilterContext* filt_asrc = nullptr, * filt_asink = nullptr;
    char aresample_swr_opts[512] = "";
    AVDictionaryEntry* e = nullptr;
    char asrc_args[256];
    int ret;

    avfilter_graph_free(&this->agraph);
    if (!(this->agraph = avfilter_graph_alloc()))
        return AVERROR(ENOMEM);
    this->agraph->nb_threads = filter_nbthreads;

    while ((e = av_dict_get(swr_opts, "", e, AV_DICT_IGNORE_SUFFIX)))
        av_strlcatf(aresample_swr_opts, sizeof(aresample_swr_opts), "%s=%s:", e->key, e->value);
    if (strlen(aresample_swr_opts))
        aresample_swr_opts[strlen(aresample_swr_opts) - 1] = '\0';
    av_opt_set(this->agraph, "aresample_swr_opts", aresample_swr_opts, 0);

    ret = snprintf(asrc_args, sizeof(asrc_args),
        "sample_rate=%d:sample_fmt=%s:channels=%d:time_base=%d/%d",
        this->audio_filter_src.freq, av_get_sample_fmt_name(this->audio_filter_src.fmt),
        this->audio_filter_src.channels,
        1, this->audio_filter_src.freq);
    if (this->audio_filter_src.channel_layout)
        snprintf(asrc_args + ret, sizeof(asrc_args) - ret, ":channel_layout=0x%"PRIx64, this->audio_filter_src.channel_layout);

    ret = avfilter_graph_create_filter(&filt_asrc, avfilter_get_by_name("abuffer"), "ffplay_abuffer", asrc_args, nullptr, this->agraph);
    if (ret < 0)
        goto end;

    ret = avfilter_graph_create_filter(&filt_asink,
        avfilter_get_by_name("abuffersink"), "ffplay_abuffersink",
        nullptr, nullptr, this->agraph);
    if (ret < 0)
        goto end;

    if ((ret = av_opt_set_int_list(filt_asink, "sample_fmts", sample_fmts, AV_SAMPLE_FMT_NONE, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;
    if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 1, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;

    if (force_output_format) {
        channel_layouts[0] = this->audio_tgt.channel_layout;
        channels[0] = this->audio_tgt.channels;
        sample_rates[0] = this->audio_tgt.freq;
        if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 0, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_layouts", channel_layouts, -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_counts", channels, -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "sample_rates", sample_rates, -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
    }


    if ((ret = configure_filtergraph(this->agraph, afilters, filt_asrc, filt_asink)) < 0)
        goto end;

    this->in_audio_filter = filt_asrc;
    this->out_audio_filter = filt_asink;

end:
    if (ret < 0)
        avfilter_graph_free(&this->agraph);
    return ret;
}
#endif  /* CONFIG_AVFILTER */

int FMediaPlayer::audio_thread()
{
    AVFrame* frame = av_frame_alloc();
    SFrame* af = nullptr;
#if CONFIG_AVFILTER
    int last_serial = -1;
    int64_t dec_channel_layout;
    int reconfigure;
#endif
    int got_frame = 0;
    AVRational tb;
    int ret = 0;

    if (!frame)
        return AVERROR(ENOMEM);

    do {
        if ((got_frame = this->auddec.Decode(frame, nullptr)) < 0)
            goto the_end;

        if (got_frame) {
            tb = AVRational{ 1, frame->sample_rate };

#if CONFIG_AVFILTER
            dec_channel_layout = get_valid_channel_layout(frame->channel_layout, frame->channels);

            reconfigure = cmp_audio_fmts(this->audio_filter_src.fmt, this->audio_filter_src.channels,
                                         static_cast<AVSampleFormat>(frame->format), frame->channels) ||
                                         this->audio_filter_src.channel_layout != dec_channel_layout ||
                                         this->audio_filter_src.freq != frame->sample_rate ||
                                         this->auddec.pkt_serial != last_serial;

            if (reconfigure) {
                char buf1[1024], buf2[1024];
                av_get_channel_layout_string(buf1, sizeof(buf1), -1, this->audio_filter_src.channel_layout);
                av_get_channel_layout_string(buf2, sizeof(buf2), -1, dec_channel_layout);
                av_log(nullptr, AV_LOG_DEBUG,
                    "Audio frame changed from rate:%d ch:%d fmt:%s layout:%s serial:%d to rate:%d ch:%d fmt:%s layout:%s serial:%d\n",
                    this->audio_filter_src.freq, this->audio_filter_src.channels, av_get_sample_fmt_name(this->audio_filter_src.fmt), buf1, last_serial,
                    frame->sample_rate, frame->channels, av_get_sample_fmt_name(static_cast<AVSampleFormat>(frame->format)), buf2, this->auddec.pkt_serial);

                this->audio_filter_src.fmt = static_cast<AVSampleFormat>(frame->format);
                this->audio_filter_src.channels = frame->channels;
                this->audio_filter_src.channel_layout = dec_channel_layout;
                this->audio_filter_src.freq = frame->sample_rate;
                last_serial = this->auddec.pkt_serial;

                if ((ret = configure_audio_filters(afilters, 1)) < 0)
                    goto the_end;
            }

            if ((ret = av_buffersrc_add_frame(this->in_audio_filter, frame)) < 0)
                goto the_end;

            while ((ret = av_buffersink_get_frame_flags(this->out_audio_filter, frame, 0)) >= 0) {
                tb = av_buffersink_get_time_base(this->out_audio_filter);
#endif
                if (!(af = this->sampq.PeekWritable()))
                    goto the_end;

                af->pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
                af->pos = frame->pkt_pos;
                af->serial = this->auddec.pkt_serial;
                af->duration = av_q2d(AVRational{ frame->nb_samples, frame->sample_rate });

                av_frame_move_ref(af->frame, frame);
                this->sampq.Push();

#if CONFIG_AVFILTER
                if (this->audioq.serial != this->auddec.pkt_serial)
                    break;
            }
            if (ret == AVERROR_EOF)
                this->auddec.finished = this->auddec.pkt_serial;
#endif
        }
    } while (ret >= 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
the_end:
#if CONFIG_AVFILTER
    avfilter_graph_free(&this->agraph);
#endif
    av_frame_free(&frame);
    return ret;
}

int FMediaPlayer::video_thread()
{
    AVFrame* frame = av_frame_alloc();
    double pts = 0.0;
    double duration = 0.0;
    int ret = -1;
    AVRational tb = this->pVideoStream->time_base;
    AVRational frame_rate = av_guess_frame_rate(this->pFormatCtx, this->pVideoStream, nullptr);

#if CONFIG_AVFILTER
    AVFilterGraph* graph = nullptr;
    AVFilterContext* filt_out = nullptr, * filt_in = nullptr;
    int last_w = 0;
    int last_h = 0;
    enum AVPixelFormat last_format = static_cast<AVPixelFormat>(-2);
    int last_serial = -1;
    int last_vfilter_idx = 0;
#endif

    if (!frame)
        return AVERROR(ENOMEM);

    for (;;)
    {
        ret = this->get_video_frame(frame);
        if (ret < 0)
            goto the_end;
        if (!ret)
            continue;

#if CONFIG_AVFILTER
        if (last_w != frame->width
            || last_h != frame->height
            || last_format != frame->format
            || last_serial != this->viddec.pkt_serial
            || last_vfilter_idx != this->vfilter_idx) {
            av_log(nullptr, AV_LOG_DEBUG,
                "Video frame changed from size:%dx%d format:%s serial:%d to size:%dx%d format:%s serial:%d\n",
                last_w, last_h,
                (const char*)av_x_if_null(av_get_pix_fmt_name(last_format), "none"), last_serial,
                frame->width, frame->height,
                (const char*)av_x_if_null(av_get_pix_fmt_name(static_cast<AVPixelFormat>(frame->format)), "none"), this->viddec.pkt_serial);
            avfilter_graph_free(&graph);
            graph = avfilter_graph_alloc();
            if (!graph) {
                ret = AVERROR(ENOMEM);
                goto the_end;
            }
            graph->nb_threads = filter_nbthreads;
            if ((ret = configure_video_filters(graph, vfilters_list ? vfilters_list[this->vfilter_idx] : nullptr, frame)) < 0) {
                SDL_Event event;
                event.type = FF_QUIT_EVENT;
                event.user.data1 = this;
                SDL_PushEvent(&event);
                goto the_end;
            }
            filt_in = this->in_video_filter;
            filt_out = this->out_video_filter;
            last_w = frame->width;
            last_h = frame->height;
            last_format = static_cast<AVPixelFormat>(frame->format);
            last_serial = this->viddec.pkt_serial;
            last_vfilter_idx = this->vfilter_idx;
            frame_rate = av_buffersink_get_frame_rate(filt_out);
        }

        ret = av_buffersrc_add_frame(filt_in, frame);
        if (ret < 0)
            goto the_end;

        while (ret >= 0) {
            this->frame_last_returned_time = av_gettime_relative() / 1000000.0;

            ret = av_buffersink_get_frame_flags(filt_out, frame, 0);
            if (ret < 0) {
                if (ret == AVERROR_EOF)
                    this->viddec.finished = this->viddec.pkt_serial;
                ret = 0;
                break;
            }

            this->frame_last_filter_delay = av_gettime_relative() / 1000000.0 - this->frame_last_returned_time;
            if (fabs(this->frame_last_filter_delay) > AV_NOSYNC_THRESHOLD / 10.0)
                this->frame_last_filter_delay = 0;
            tb = av_buffersink_get_time_base(filt_out);
#endif
            duration = (frame_rate.num && frame_rate.den ? av_q2d(AVRational{ frame_rate.den, frame_rate.num }) : 0);
            pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
            ret = this->queue_picture(frame, pts, duration, frame->pkt_pos, this->viddec.pkt_serial);
            av_frame_unref(frame);
#if CONFIG_AVFILTER
            if (this->videoq.serial != this->viddec.pkt_serial)
                break;
        }
#endif

        if (ret < 0)
            goto the_end;
    }
the_end:
#if CONFIG_AVFILTER
    avfilter_graph_free(&graph);
#endif
    av_frame_free(&frame);
    return 0;
}

int FMediaPlayer::subtitle_thread()
{
    SFrame* sp = nullptr;
    int got_subtitle;
    double pts;

    for (;;)
    {
        if (!(sp = this->subpq.PeekWritable()))
            return 0;

        if ((got_subtitle = this->subdec.Decode(nullptr, &sp->sub)) < 0)
            break;

        pts = 0;

        if (got_subtitle && sp->sub.format == 0)
        {
            if (sp->sub.pts != AV_NOPTS_VALUE)
                pts = sp->sub.pts / (double)AV_TIME_BASE;
            sp->pts = pts;
            sp->serial = this->subdec.pkt_serial;
            sp->width = this->subdec.pAvCtx->width;
            sp->height = this->subdec.pAvCtx->height;
            sp->uploaded = false;

            /* now we can update the picture count */
            this->subpq.Push();
        }
        else if (got_subtitle) {
            avsubtitle_free(&sp->sub);
        }
    }
    return 0;
}

AVDictionary** FMediaPlayer::setup_find_stream_info_opts(AVFormatContext* s, AVDictionary* codec_opts)
{
    auto check_stream_specifier = [](AVFormatContext* s, AVStream* st, const char* spec) -> int
    {
        int ret = avformat_match_stream_specifier(s, st, spec);
        if (ret < 0)
            av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
        return ret;
    };

    auto filter_codec_opts = [&](AVDictionary* opts, enum AVCodecID codec_id,
        AVFormatContext* s, AVStream* st, AVCodec* codec) -> AVDictionary*
    {
        AVDictionary* ret = NULL;
        AVDictionaryEntry* t = NULL;
        int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
            : AV_OPT_FLAG_DECODING_PARAM;
        char          prefix = 0;
        const AVClass* cc = avcodec_get_class();

        if (!codec)
            codec = s->oformat ? avcodec_find_encoder(codec_id)
            : avcodec_find_decoder(codec_id);

        switch (st->codecpar->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
            prefix = 'v';
            flags |= AV_OPT_FLAG_VIDEO_PARAM;
            break;
        case AVMEDIA_TYPE_AUDIO:
            prefix = 'a';
            flags |= AV_OPT_FLAG_AUDIO_PARAM;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            prefix = 's';
            flags |= AV_OPT_FLAG_SUBTITLE_PARAM;
            break;
        }

        while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
            char* p = strchr(t->key, ':');

            /* check stream specification in opt name */
            if (p)
                switch (check_stream_specifier(s, st, p + 1)) {
                case  1: *p = 0; break;
                case  0:         continue;
                default:         return nullptr;
                }

            if (av_opt_find(&cc, t->key, NULL, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
                !codec ||
                (codec->priv_class &&
                    av_opt_find(&codec->priv_class, t->key, NULL, flags,
                        AV_OPT_SEARCH_FAKE_OBJ)))
                av_dict_set(&ret, t->key, t->value, 0);
            else if (t->key[0] == prefix &&
                av_opt_find(&cc, t->key + 1, NULL, flags,
                    AV_OPT_SEARCH_FAKE_OBJ))
                av_dict_set(&ret, t->key + 1, t->value, 0);

            if (p)
                *p = ':';
        }
        return ret;
    };

    AVDictionary** opts;

    if (!s->nb_streams)
        return NULL;
    opts = static_cast<AVDictionary**>(av_mallocz_array(s->nb_streams, sizeof(*opts)));
    if (!opts) {
        av_log(NULL, AV_LOG_ERROR,
            "Could not alloc memory for stream options.\n");
        return NULL;
    }
    for (uint32_t i = 0; i < s->nb_streams; i++)
        opts[i] = filter_codec_opts(codec_opts, s->streams[i]->codecpar->codec_id,
            s, s->streams[i], NULL);
    return opts;
}

int FMediaPlayer::read_thread()
{
    AVFormatContext* ic = nullptr;
    int err, i, ret;
    int st_index[AVMEDIA_TYPE_NB];
    AVPacket pkt1, * pkt = &pkt1;
    int64_t stream_start_time;
    int pkt_in_play_range = 0;
    std::mutex wait_mutex;
    int scan_all_pmts_set = 0;
    int64_t pkt_ts;

    AVDictionary* format_opts = nullptr;
    AVDictionary* codec_opts = nullptr;

    memset(st_index, -1, sizeof(st_index));
    this->last_video_stream = this->video_stream = -1;
    this->last_audio_stream = this->audio_stream = -1;
    this->last_subtitle_stream = this->subtitle_stream = -1;
    this->eof = false;

    ic = avformat_alloc_context();
    if (!ic) {
        av_log(nullptr, AV_LOG_FATAL, "Could not allocate context.\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    /* ????????????????? */
    ic->interrupt_callback.callback = decode_interrupt_cb;
    ic->interrupt_callback.opaque = this;

    av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
    err = avformat_open_input(&ic, this->sURL.c_str(), this->pInputformat, &format_opts);
    if (err < 0) {
        char errbuf[128];
        const char* errbuf_ptr = errbuf;

        if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
            errbuf_ptr = strerror(AVUNERROR(err));

        av_log(nullptr, AV_LOG_ERROR, "%s: %s\n", this->sURL.c_str(), errbuf_ptr);
        ret = -1;
        goto fail;
    }
    if (scan_all_pmts_set)
        av_dict_set(&format_opts, "scan_all_pmts", nullptr, AV_DICT_MATCH_CASE);

    this->pFormatCtx = ic;

    av_format_inject_global_side_data(ic);

    {
        AVDictionary** opts = setup_find_stream_info_opts(ic, codec_opts);
        const unsigned int orig_nb_streams = ic->nb_streams;

        err = avformat_find_stream_info(ic, opts);

        for (i = 0; i < orig_nb_streams; i++)
            av_dict_free(&opts[i]);
        av_freep(&opts);

        if (err < 0) {
            av_log(nullptr, AV_LOG_WARNING, "%s: could not find codec parameters\n", this->sURL.c_str());
            ret = -1;
            goto fail;
        }
    }

    if (ic->pb)
        ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end

    if (!this->seek_by_bytes)
        this->seek_by_bytes = !!(ic->iformat->flags & AVFMT_TS_DISCONT) && strcmp("ogg", ic->iformat->name);

    this->max_frame_duration = (ic->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

    /* if seeking requested, we execute it */
    if (this->start_time != AV_NOPTS_VALUE)
    {
        int64_t timestamp = this->start_time;

        /* add the stream start time */
        if (ic->start_time != AV_NOPTS_VALUE)
            timestamp += ic->start_time;
        ret = avformat_seek_file(ic, -1, INT64_MIN, timestamp, INT64_MAX, 0);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_WARNING, "%s: could not seek to position %0.3f\n", this->sURL.c_str(), (double)timestamp / AV_TIME_BASE);
        }
    }

    /* ????????????????? */
    av_dump_format(ic, 0, this->sURL.c_str(), 0);

    for (uint32_t i = 0; i < ic->nb_streams; i++) {
        AVStream* st = ic->streams[i];
        enum AVMediaType type = st->codecpar->codec_type;
        st->discard = AVDISCARD_ALL;
        if (type >= 0 && wanted_stream_spec[type] && st_index[type] == -1)
            if (avformat_match_stream_specifier(ic, st, wanted_stream_spec[type]) > 0)
                st_index[type] = i;
    }
    for (uint32_t i = 0; i < AVMEDIA_TYPE_NB; i++) {
        if (wanted_stream_spec[i] && st_index[i] == -1) {
            av_log(nullptr, AV_LOG_ERROR, "Stream specifier %s does not match any %s stream\n", wanted_stream_spec[i], av_get_media_type_string(static_cast<AVMediaType>(i)));
            st_index[i] = INT_MAX;
        }
    }

    st_index[AVMEDIA_TYPE_VIDEO] = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, st_index[AVMEDIA_TYPE_VIDEO], -1, nullptr, 0);
    st_index[AVMEDIA_TYPE_AUDIO] = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, st_index[AVMEDIA_TYPE_AUDIO], st_index[AVMEDIA_TYPE_VIDEO], nullptr, 0);
    st_index[AVMEDIA_TYPE_SUBTITLE] = av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE, st_index[AVMEDIA_TYPE_SUBTITLE],
        (st_index[AVMEDIA_TYPE_AUDIO] >= 0 ? st_index[AVMEDIA_TYPE_AUDIO] : st_index[AVMEDIA_TYPE_VIDEO]),
        nullptr, 0);

    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        AVStream* st = ic->streams[st_index[AVMEDIA_TYPE_VIDEO]];
        AVRational sar = av_guess_sample_aspect_ratio(ic, st, nullptr);
        if (st->codecpar->width)
            this->OnWindowSizeChange(st->codecpar->width, st->codecpar->height, sar);
    }

    /* open the streams */
    if (st_index[AVMEDIA_TYPE_AUDIO] >= 0)
        this->stream_component_open(st_index[AVMEDIA_TYPE_AUDIO]);

    ret = -1;
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0)
        ret = this->stream_component_open(st_index[AVMEDIA_TYPE_VIDEO]);

    if (this->eShow_mode == FMediaPlayer::EShowMode::SHOW_MODE_NONE)
        this->eShow_mode = ret >= 0 ? FMediaPlayer::EShowMode::SHOW_MODE_VIDEO : FMediaPlayer::EShowMode::SHOW_MODE_RDFT;

    if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0)
        this->stream_component_open(st_index[AVMEDIA_TYPE_SUBTITLE]);

    if (this->video_stream < 0 && this->audio_stream < 0) {
        av_log(nullptr, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n", this->sURL.c_str());
        ret = -1;
        goto fail;
    }

    if (!this->infinite_buffer && this->is_realtime())
        this->infinite_buffer = true;

    for (;;)
    {
        /* ??????? */
        if (this->abort_request)
            break;

        /* ???/???? */
        if (this->paused != this->last_paused)
        {
            this->last_paused = this->paused;
            if (this->paused)
                this->read_pause_return = av_read_pause(ic);
            else
                av_read_play(ic);
        }
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
        if (this->paused &&
            (!strcmp(ic->iformat->name, "rtsp") ||
            (ic->pb && !strncmp(this->sURL.c_str(), "mmsh:", 5))))
        {
            /* wait 10 ms to avoid trying to get another packet */
            /* XXX: horrible */
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
#endif
        if (this->seek_req) {
            int64_t seek_target = this->seek_pos;
            int64_t seek_min = this->seek_rel > 0 ? seek_target - this->seek_rel + 2 : INT64_MIN;
            int64_t seek_max = this->seek_rel < 0 ? seek_target - this->seek_rel - 2 : INT64_MAX;
            // FIXME the +-2 pPlayer due to rounding being not done in the correct direction in generation
            //      of the seek_pos/seek_rel variables

            ret = avformat_seek_file(this->pFormatCtx, -1, seek_min, seek_target, seek_max, this->seek_flags);
            if (ret < 0) {
                av_log(nullptr, AV_LOG_ERROR, "%s: error while seeking\n", this->sURL.c_str()/*this->pFormatCtx->url*/);
            }
            else
            {
                if (this->audio_stream >= 0) {
                    this->audioq.Flush();
                    this->audioq.Put(&flush_pkt);
                }
                if (this->subtitle_stream >= 0) {
                    this->subtitleq.Flush();
                    this->subtitleq.Put(&flush_pkt);
                }
                if (this->video_stream >= 0) {
                    this->videoq.Flush();
                    this->videoq.Put(&flush_pkt);
                }
                if (this->seek_flags & AVSEEK_FLAG_BYTE)
                    this->extclk.Set(NAN, 0);
                else
                    this->extclk.Set(seek_target / (double)AV_TIME_BASE, 0);
            }
            this->seek_req = false;
            this->queue_attachments_req = true;
            this->eof = false;
            if (this->paused)
                this->OnStepToNextFrame();
        }
        if (this->queue_attachments_req) {
            if (this->pVideoStream && this->pVideoStream->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                AVPacket copy;
                if ((ret = av_packet_ref(&copy, &this->pVideoStream->attached_pic)) < 0)
                    goto fail;
                this->videoq.Put(&copy);
                this->videoq.PutNullPacket(this->video_stream);
            }
            this->queue_attachments_req = false;
        }

        /* if the queue are full, no need to read more */
        if (!this->infinite_buffer &&
            (this->audioq.size + this->videoq.size + this->subtitleq.size > MAX_QUEUE_SIZE
                || (this->stream_has_enough_packets(this->pAudioStream, this->audio_stream, &this->audioq) &&
                    this->stream_has_enough_packets(this->pVideoStream, this->video_stream, &this->videoq) &&
                    this->stream_has_enough_packets(this->pSubtitleStream, this->subtitle_stream, &this->subtitleq))))
        {

            std::unique_lock<std::mutex> lock(wait_mutex);
            this->continue_read_thread->wait_for(lock, std::chrono::milliseconds(10));
            continue;
        }
        if (!this->paused &&
            (!this->pAudioStream || (this->auddec.finished == this->audioq.serial && this->sampq.NbRemaining() == 0)) &&
            (!this->pVideoStream || (this->viddec.finished == this->videoq.serial && this->pictq.NbRemaining() == 0))) {
            if (this->loop != 1 && (!this->loop || --this->loop))
                this->OnStreamSeek(this->start_time != AV_NOPTS_VALUE ? this->start_time : 0, 0, false);

            else if (autoexit) {
                ret = AVERROR_EOF;
                goto fail;
            }
        }
        ret = av_read_frame(ic, pkt);
        if (ret < 0)
        {
            /* ?????????? */
            if ((ret == AVERROR_EOF || avio_feof(ic->pb)) && !this->eof)
            {
                if (this->video_stream >= 0)
                    this->videoq.PutNullPacket(this->video_stream);
                if (this->audio_stream >= 0)
                    this->audioq.PutNullPacket(this->audio_stream);
                if (this->subtitle_stream >= 0)
                    this->subtitleq.PutNullPacket(this->subtitle_stream);
                this->eof = true;
            }
            if (ic->pb && ic->pb->error)
                break;

            std::unique_lock<std::mutex> lock(wait_mutex);
            this->continue_read_thread->wait_for(lock, std::chrono::milliseconds(10));
            continue;
        }
        else this->eof = false;

        /* check if packet pPlayer in play range specified by user, then queue, otherwise discard */
        stream_start_time = ic->streams[pkt->stream_index]->start_time;
        pkt_ts = pkt->pts == AV_NOPTS_VALUE ? pkt->dts : pkt->pts;
        pkt_in_play_range = this->duration == AV_NOPTS_VALUE ||
            (pkt_ts - (stream_start_time != AV_NOPTS_VALUE ? stream_start_time : 0)) *
            av_q2d(ic->streams[pkt->stream_index]->time_base) -
            (double)(this->start_time != AV_NOPTS_VALUE ? this->start_time : 0) / 1000000
            <= ((double)this->duration / 1000000);
        if (pkt->stream_index == this->audio_stream && pkt_in_play_range) {
            this->audioq.Put(pkt);
        }
        else if (pkt->stream_index == this->video_stream && pkt_in_play_range
            && !(this->pVideoStream->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
            this->videoq.Put(pkt);
        }
        else if (pkt->stream_index == this->subtitle_stream && pkt_in_play_range) {
            this->subtitleq.Put(pkt);
        }
        else av_packet_unref(pkt);
    }

    ret = 0;
fail:
    if (ic && !this->pFormatCtx)
        avformat_close_input(&ic);

    if(format_opts)
        av_dict_free(&format_opts);

    if (ret != 0) {
        SDL_Event event;

        event.type = FF_QUIT_EVENT;
        event.user.data1 = this;
        SDL_PushEvent(&event);
    }
    return 0;
}

int FMediaPlayer::audio_open(int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams* audio_hw_params)
{
    SDL_AudioSpec wanted_spec, spec;
    const char* env;
    static const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };
    static const int next_sample_rates[] = { 0, 44100, 48000, 96000, 192000 };
    int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;

    env = SDL_getenv("SDL_AUDIO_CHANNELS");
    if (env) {
        wanted_nb_channels = std::atoi(env);
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
    }
    if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
    }
    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
    wanted_spec.channels = wanted_nb_channels;
    wanted_spec.freq = wanted_sample_rate;
    if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
        av_log(nullptr, AV_LOG_ERROR, "Invalid sample rate or channel count!\n");
        return -1;
    }
    while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_spec.freq)
        next_sample_rate_idx--;

    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wanted_spec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
    /* ?????????? */
    wanted_spec.callback = FMediaPlayer::sdl_audio_callback;
    wanted_spec.userdata = this;

    SDL_GetNumAudioDrivers();
    //SDL_GetAudioDeviceName();

    /* ????????υτ */
    while (!(audio_dev = SDL_OpenAudioDevice(nullptr, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE)))
    {
        av_log(nullptr, AV_LOG_WARNING, "SDL_OpenAudio (%d channels, %d Hz): %s\n", wanted_spec.channels, wanted_spec.freq, SDL_GetError());
        wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
        if (!wanted_spec.channels) {
            wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
            wanted_spec.channels = wanted_nb_channels;
            if (!wanted_spec.freq) {
                av_log(nullptr, AV_LOG_ERROR,
                    "No more combinations to try, audio open failed\n");
                return -1;
            }
        }
        wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
    }
    if (spec.format != AUDIO_S16SYS) {
        av_log(nullptr, AV_LOG_ERROR, "SDL advised audio format %d is not supported!\n", spec.format);
        return -1;
    }
    if (spec.channels != wanted_spec.channels) {
        wanted_channel_layout = av_get_default_channel_layout(spec.channels);
        if (!wanted_channel_layout) {
            av_log(nullptr, AV_LOG_ERROR, "SDL advised channel count %d is not supported!\n", spec.channels);
            return -1;
        }
    }

    audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
    audio_hw_params->freq = spec.freq;
    audio_hw_params->channel_layout = wanted_channel_layout;
    audio_hw_params->channels = spec.channels;
    audio_hw_params->frame_size = av_samples_get_buffer_size(nullptr, audio_hw_params->channels, 1, audio_hw_params->fmt, 1);
    audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(nullptr, audio_hw_params->channels, audio_hw_params->freq, audio_hw_params->fmt, 1);
    if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
        av_log(nullptr, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
        return -1;
    }
    return spec.size;
}

void FMediaPlayer::sdl_audio_callback(void* pUserData, Uint8* stream, int len)
{
    FMediaPlayer* pPlayer = static_cast<FMediaPlayer*>(pUserData);
    int audio_size, len1;

    pPlayer->audio_callback_time = av_gettime_relative();

    while (len > 0)
    {
        if (pPlayer->audio_buf_index >= pPlayer->audio_buf_size)
        {
            audio_size = pPlayer->audio_decode_frame();
            if (audio_size < 0) {
                /* if error, just output silence */
                pPlayer->audio_buf = nullptr;
                pPlayer->audio_buf_size = SDL_AUDIO_MIN_BUFFER_SIZE / pPlayer->audio_tgt.frame_size * pPlayer->audio_tgt.frame_size;
            }
            else {
                if (pPlayer->eShow_mode != FMediaPlayer::EShowMode::SHOW_MODE_VIDEO)
                    pPlayer->update_sample_display((int16_t*)pPlayer->audio_buf, audio_size);
                pPlayer->audio_buf_size = audio_size;
            }
            pPlayer->audio_buf_index = 0;
        }
        len1 = pPlayer->audio_buf_size - pPlayer->audio_buf_index;
        if (len1 > len)
            len1 = len;
        if (!pPlayer->muted && pPlayer->audio_buf && pPlayer->audio_volume == SDL_MIX_MAXVOLUME)
            memcpy(stream, (uint8_t*)pPlayer->audio_buf + pPlayer->audio_buf_index, len1);
        else {
            memset(stream, 0, len1);
            /* ???????? */
            if (!pPlayer->muted && pPlayer->audio_buf)
                SDL_MixAudioFormat(stream, (uint8_t*)pPlayer->audio_buf + pPlayer->audio_buf_index, AUDIO_S16SYS, len1, pPlayer->audio_volume);
        }
        len -= len1;
        stream += len1;	/* ??????? */
        pPlayer->audio_buf_index += len1;
    }
    pPlayer->audio_write_buf_size = pPlayer->audio_buf_size - pPlayer->audio_buf_index;
    /* Let's assume the audio driver that is used by SDL has two periods. */
    if (!std::isnan(pPlayer->audio_clock))
    {
        pPlayer->audclk.SetAt(pPlayer->audio_clock - (double)(2 * pPlayer->audio_hw_buf_size + pPlayer->audio_write_buf_size) / pPlayer->audio_tgt.bytes_per_sec, pPlayer->audio_clock_serial, pPlayer->audio_callback_time / 1000000.0);
        sync_clock_to_slave(pPlayer->extclk, pPlayer->audclk);
    }
}

int FMediaPlayer::decode_interrupt_cb(void* pUserData)
{
    FMediaPlayer* pPlayer = static_cast<FMediaPlayer*>(pUserData);
    return pPlayer->abort_request;
}

void FMediaPlayer::update_sample_display(short* samples, int samples_size)
{
    int size, len;

    size = samples_size / sizeof(short);
    while (size > 0)
    {
        len = SAMPLE_ARRAY_SIZE - this->sample_array_index;
        if (len > size)
            len = size;
        memcpy(this->sample_array + this->sample_array_index, samples, len * sizeof(short));
        samples += len;
        this->sample_array_index += len;
        if (this->sample_array_index >= SAMPLE_ARRAY_SIZE)
            this->sample_array_index = 0;
        size -= len;
    }
}

int FMediaPlayer::synchronize_audio(int nb_samples)
{
    int wanted_nb_samples = nb_samples;

    /* if not master, then we try to remove or add samples to correct the clock */
    if (get_master_sync_type() != ESyncType::AV_SYNC_AUDIO_MASTER)
    {
        double diff, avg_diff;
        int min_nb_samples, max_nb_samples;

        diff = this->audclk.Get() - get_master_clock();

        if (!std::isnan(diff) && std::fabs(diff) < AV_NOSYNC_THRESHOLD) {
            this->audio_diff_cum = diff + this->audio_diff_avg_coef * this->audio_diff_cum;
            if (this->audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
                /* not enough measures to have a correct estimate */
                this->audio_diff_avg_count++;
            }
            else {
                /* estimate the A-V difference */
                avg_diff = this->audio_diff_cum * (1.0 - this->audio_diff_avg_coef);

                if (std::fabs(avg_diff) >= this->audio_diff_threshold) {
                    wanted_nb_samples = nb_samples + (int)(diff * this->audio_src.freq);
                    min_nb_samples = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100));
                    max_nb_samples = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100));
                    wanted_nb_samples = av_clip(wanted_nb_samples, min_nb_samples, max_nb_samples);
                }
                av_log(nullptr, AV_LOG_TRACE, "diff=%f adiff=%f sample_diff=%d apts=%0.3f %f\n",
                    diff, avg_diff, wanted_nb_samples - nb_samples,
                    this->audio_clock, this->audio_diff_threshold);
            }
        }
        else {
            /* too big difference : may be initial PTS errors, so
               reset A-V filter */
            this->audio_diff_avg_count = 0;
            this->audio_diff_cum = 0;
        }
    }

    return wanted_nb_samples;
}

int FMediaPlayer::audio_decode_frame()
{
    int data_size, resampled_data_size;
    int64_t dec_channel_layout;
    av_unused double audio_clock0;
    int wanted_nb_samples;
    SFrame* af = nullptr;

    if (this->paused)
        return -1;

    do {
#if defined(_WIN32)
        while (this->sampq.NbRemaining() == 0) {
            if ((av_gettime_relative() - audio_callback_time) > 1000000LL * this->audio_hw_buf_size / this->audio_tgt.bytes_per_sec / 2)
                return -1;
            av_usleep(1000);
        }
#endif
        if (!(af = this->sampq.PeekReadable()))
            return -1;
        this->sampq.Next();
    } while (af->serial != this->audioq.serial);

    data_size = av_samples_get_buffer_size(nullptr, af->frame->channels, af->frame->nb_samples, static_cast<AVSampleFormat>(af->frame->format), 1);

    dec_channel_layout =
        (af->frame->channel_layout && af->frame->channels == av_get_channel_layout_nb_channels(af->frame->channel_layout)) ?
        af->frame->channel_layout : av_get_default_channel_layout(af->frame->channels);
    wanted_nb_samples = synchronize_audio(af->frame->nb_samples);

    if (af->frame->format != this->audio_src.fmt ||
        dec_channel_layout != this->audio_src.channel_layout ||
        af->frame->sample_rate != this->audio_src.freq ||
        (wanted_nb_samples != af->frame->nb_samples && !this->swr_ctx))
    {
        swr_free(&this->swr_ctx);
        this->swr_ctx = swr_alloc_set_opts(nullptr,
            this->audio_tgt.channel_layout, this->audio_tgt.fmt, this->audio_tgt.freq,
            dec_channel_layout, static_cast<AVSampleFormat>(af->frame->format), af->frame->sample_rate,
            0, nullptr);
        if (!this->swr_ctx || swr_init(this->swr_ctx) < 0)
        {
            av_log(nullptr, AV_LOG_ERROR,
                "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                af->frame->sample_rate, av_get_sample_fmt_name(static_cast<AVSampleFormat>(af->frame->format)), af->frame->channels,
                this->audio_tgt.freq, av_get_sample_fmt_name(this->audio_tgt.fmt), this->audio_tgt.channels);
            swr_free(&this->swr_ctx);
            return -1;
        }
        this->audio_src.channel_layout = dec_channel_layout;
        this->audio_src.channels = af->frame->channels;
        this->audio_src.freq = af->frame->sample_rate;
        this->audio_src.fmt = static_cast<AVSampleFormat>(af->frame->format);
    }

    if (this->swr_ctx)
    {
        const uint8_t** in = (const uint8_t**)af->frame->extended_data;
        uint8_t** out = &this->audio_buf1;
        int out_count = (int64_t)wanted_nb_samples * this->audio_tgt.freq / af->frame->sample_rate + 256;
        int out_size = av_samples_get_buffer_size(nullptr, this->audio_tgt.channels, out_count, this->audio_tgt.fmt, 0);
        int len2;
        if (out_size < 0) {
            av_log(nullptr, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
            return -1;
        }
        if (wanted_nb_samples != af->frame->nb_samples) {
            if (swr_set_compensation(this->swr_ctx, (wanted_nb_samples - af->frame->nb_samples) * this->audio_tgt.freq / af->frame->sample_rate,
                wanted_nb_samples * this->audio_tgt.freq / af->frame->sample_rate) < 0) {
                av_log(nullptr, AV_LOG_ERROR, "swr_set_compensation() failed\n");
                return -1;
            }
        }
        av_fast_malloc(&this->audio_buf1, &this->audio_buf1_size, out_size);
        if (!this->audio_buf1)
            return AVERROR(ENOMEM);
        len2 = swr_convert(this->swr_ctx, out, out_count, in, af->frame->nb_samples);
        if (len2 < 0) {
            av_log(nullptr, AV_LOG_ERROR, "swr_convert() failed\n");
            return -1;
        }
        if (len2 == out_count) {
            av_log(nullptr, AV_LOG_WARNING, "audio buffer is probably too small\n");
            if (swr_init(this->swr_ctx) < 0)
                swr_free(&this->swr_ctx);
        }
        this->audio_buf = this->audio_buf1;
        resampled_data_size = len2 * this->audio_tgt.channels * av_get_bytes_per_sample(this->audio_tgt.fmt);
    }
    else {
        this->audio_buf = af->frame->data[0];
        resampled_data_size = data_size;
    }

    audio_clock0 = this->audio_clock;
    /* update the audio clock with the pts */
    if (!isnan(af->pts))
        this->audio_clock = af->pts + (double)af->frame->nb_samples / af->frame->sample_rate;
    else
        this->audio_clock = NAN;
    this->audio_clock_serial = af->serial;
#ifdef DEBUG
    {
        static double last_clock;
        printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
            this->audio_clock - last_clock,
            this->audio_clock, audio_clock0);
        last_clock = this->audio_clock;
    }
#endif
    return resampled_data_size;
}

int FMediaPlayer::stream_component_open(int stream_index)
{
    AVCodecContext* avctx = nullptr;
    AVCodec* codec = nullptr;
    AVDictionary* opts = nullptr;
    int sample_rate, nb_channels;
    int64_t channel_layout;
    int ret = 0;
    int stream_lowres = lowres;

    if (stream_index < 0 || stream_index >= pFormatCtx->nb_streams)
        return -1;

    avctx = avcodec_alloc_context3(nullptr);
    if (!avctx)
        return AVERROR(ENOMEM);

    ret = avcodec_parameters_to_context(avctx, pFormatCtx->streams[stream_index]->codecpar);
    if (ret < 0)
        goto fail;
    avctx->pkt_timebase = pFormatCtx->streams[stream_index]->time_base;

    codec = avcodec_find_decoder(avctx->codec_id);

    switch (avctx->codec_type)
    {
        case AVMEDIA_TYPE_AUDIO: this->last_audio_stream = stream_index; break;
        case AVMEDIA_TYPE_SUBTITLE: this->last_subtitle_stream = stream_index; break;
        case AVMEDIA_TYPE_VIDEO: this->last_video_stream = stream_index; break;
    default:
        av_log(nullptr, 4, "Codec Type not support!");
        goto fail;
    }

    avctx->codec_id = codec->id;
    if (stream_lowres > codec->max_lowres) {
        av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n", codec->max_lowres);
        stream_lowres = codec->max_lowres;
    }
    avctx->lowres = stream_lowres;

    avctx->flags2 |= AV_CODEC_FLAG2_FAST;

    /*opts = filter_codec_opts(codec_opts, avctx->codec_id, pFormatCtx, pFormatCtx->streams[stream_index], codec);
    if (!av_dict_get(opts, "threads", nullptr, 0))*/
        av_dict_set(&opts, "threads", "auto", 0);
    if (stream_lowres)
        av_dict_set_int(&opts, "lowres", stream_lowres, 0);
    if (avctx->codec_type == AVMEDIA_TYPE_VIDEO || avctx->codec_type == AVMEDIA_TYPE_AUDIO)
        av_dict_set(&opts, "refcounted_frames", "1", 0);
    if ((ret = avcodec_open2(avctx, codec, &opts)) < 0) {
        goto fail;
    }

    /* @TODO seq:1 ?????????????????????? */
    /*if ((t = av_dict_get(opts, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
        av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
        ret = AVERROR_OPTION_NOT_FOUND;
        goto fail;
    }*/

    this->eof = false;
    pFormatCtx->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    switch (avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
    {
#if CONFIG_AVFILTER

        AVFilterContext* sink = nullptr;

        this->audio_filter_src.freq = avctx->sample_rate;
        this->audio_filter_src.channels = avctx->channels;
        this->audio_filter_src.channel_layout = get_valid_channel_layout(avctx->channel_layout, avctx->channels);
        this->audio_filter_src.fmt = avctx->sample_fmt;
        if ((ret = configure_audio_filters(afilters, 0)) < 0)
            goto fail;
        sink = this->out_audio_filter;
        sample_rate = av_buffersink_get_sample_rate(sink);
        nb_channels = av_buffersink_get_channels(sink);
        channel_layout = av_buffersink_get_channel_layout(sink);
    //}
#else
        sample_rate = avctx->sample_rate;
        nb_channels = avctx->channels;
        channel_layout = static_cast<int64_t>(avctx->channel_layout);
#endif

        /* prepare audio output */
        if ((ret = audio_open(channel_layout, nb_channels, sample_rate, &this->audio_tgt)) < 0)
            goto fail;

        this->audio_hw_buf_size = ret;
        this->audio_src = this->audio_tgt;
        this->audio_buf_size = 0;
        this->audio_buf_index = 0;

        /* init averaging filter */
        this->audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
        this->audio_diff_avg_count = 0;
        /* since we do not have a precise anough audio FIFO fullness,
           we correct audio sync only if larger than this threshold */
        this->audio_diff_threshold = double(this->audio_hw_buf_size) / this->audio_tgt.bytes_per_sec;

        this->audio_stream = stream_index;
        this->pAudioStream = pFormatCtx->streams[stream_index];

        this->auddec.Init(avctx, &this->audioq, this->continue_read_thread);
        if ((this->pFormatCtx->iformat->flags & (AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH | AVFMT_NO_BYTE_SEEK)) && !this->pFormatCtx->iformat->read_seek) {
            this->auddec.start_pts = this->pAudioStream->start_time;
            this->auddec.start_pts_tb = this->pAudioStream->time_base;
        }
        std::function<int(void)> audioFunc = std::bind(&FMediaPlayer::audio_thread, this);
        if ((ret = this->auddec.Start(audioFunc)) < 0)
            goto out;
        SDL_PauseAudioDevice(audio_dev, 0);
    }
    break;
    case AVMEDIA_TYPE_VIDEO:
    {
        this->video_stream = stream_index;
        this->pVideoStream = pFormatCtx->streams[stream_index];

        this->viddec.Init(avctx, &this->videoq, this->continue_read_thread);
        std::function<int(void)> videoFunc = std::bind(&FMediaPlayer::video_thread, this);
        if ((ret = this->viddec.Start(videoFunc)) < 0)
            goto out;
        this->queue_attachments_req = true;
    }
    break;
    case AVMEDIA_TYPE_SUBTITLE:
    {
        this->subtitle_stream = stream_index;
        this->pSubtitleStream = pFormatCtx->streams[stream_index];

        this->subdec.Init(avctx, &this->subtitleq, this->continue_read_thread);
        std::function<int(void)> subtitleoFunc = std::bind(&FMediaPlayer::subtitle_thread, this);
        if ((ret = this->subdec.Start(subtitleoFunc)) < 0)
            goto out;
    }
    break;
    default:
        break;
    }
    goto out;

fail:
    avcodec_free_context(&avctx);
out:
    if(opts) av_dict_free(&opts);

    return ret;
}

bool FMediaPlayer::stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue)
{
    return stream_id < 0 ||
        queue->abort_request ||
        (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
        queue->nb_packets > MIN_FRAMES && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0);
}

bool FMediaPlayer::is_realtime()
{
    if (pFormatCtx == nullptr)
        return false;

    if (!strcmp(pFormatCtx->iformat->name, "rtp") || !strcmp(pFormatCtx->iformat->name, "rtsp") || !strcmp(pFormatCtx->iformat->name, "sdp"))
        return true;

    if (pFormatCtx->pb && (!strncmp(this->sURL.c_str()/*pFormatCtx->url*/, "rtp:", 4) || !strncmp(/*pFormatCtx->url*/this->sURL.c_str(), "udp:", 4) || !strncmp(this->sURL.c_str()/*pFormatCtx->url*/, "rtmp:", 4)))
        return true;

    return false;
}

bool FMediaPlayer::OnStreamOpen(const std::string& sURL, AVInputFormat* iformat)
{
    if (sURL.empty())
        return false;

    if (!initRender())
        return false;

    av_init_packet(&this->flush_pkt);
    this->flush_pkt.data = (uint8_t*)&this->flush_pkt;

    std::function<int(void)> readFunc = std::bind(&FMediaPlayer::read_thread, this);

    this->sURL = sURL;
    if (this->sURL.empty())
        goto fail;
    if (iformat)
        this->pInputformat = iformat;
    this->rect.y = 0;
    this->rect.x = 0;

    /* start video display */
    if (!this->pictq.Init(&this->videoq, VIDEO_PICTURE_QUEUE_SIZE, 1))
        goto fail;
    if (!this->subpq.Init(&this->subtitleq, SUBPICTURE_QUEUE_SIZE, 0))
        goto fail;
    if (!this->sampq.Init(&this->audioq, SAMPLE_QUEUE_SIZE, 1))
        goto fail;

    this->videoq.Init(this->flush_pkt);
    this->audioq.Init(this->flush_pkt);
    this->subtitleq.Init(this->flush_pkt);

    this->continue_read_thread = std::make_shared<std::condition_variable>();
    if (this->continue_read_thread == nullptr)
    {
        av_log(nullptr, AV_LOG_ERROR, "Make a condition varibale failed\n");
        return false;
    }

    this->vidclk.Init(&this->videoq.serial);
    this->audclk.Init(&this->audioq.serial);
    this->extclk.Init(&this->extclk.serial);
    this->audio_clock_serial = -1;
    this->audio_volume = 100;
    this->audio_volume = av_clip(SDL_MIX_MAXVOLUME * this->audio_volume / 100, 0, SDL_MIX_MAXVOLUME);
    this->muted = false;
    /* @TODO */
    //this->av_sync_type = av_sync_type;

    this->future = std::async(std::launch::async, std::move(readFunc));
    if (!this->future.valid())
    {
        av_log(nullptr, AV_LOG_FATAL, "CreateThread Failured\n");
    fail:
        StreamClose();
        return false;
    }
    return true;
}

void FMediaPlayer::OnStreamCycleChannel(AVMediaType codec_type)
{
    if (!pFormatCtx)
        return;

    int start_index, stream_index;
    int old_index;
    AVStream* st = nullptr;
    AVProgram* p = nullptr;
    int nb_streams = pFormatCtx->nb_streams;

    if (codec_type == AVMEDIA_TYPE_VIDEO) {
        start_index = this->last_video_stream;
        old_index = this->video_stream;
    }
    else if (codec_type == AVMEDIA_TYPE_AUDIO) {
        start_index = this->last_audio_stream;
        old_index = this->audio_stream;
    }
    else {
        start_index = this->last_subtitle_stream;
        old_index = this->subtitle_stream;
    }
    stream_index = start_index;

    if (codec_type != AVMEDIA_TYPE_VIDEO && this->video_stream != -1) {
        p = av_find_program_from_stream(pFormatCtx, nullptr, this->video_stream);
        if (p) {
            nb_streams = p->nb_stream_indexes;
            for (start_index = 0; start_index < nb_streams; start_index++)
                if (p->stream_index[start_index] == stream_index)
                    break;
            if (start_index == nb_streams)
                start_index = -1;
            stream_index = start_index;
        }
    }

    for (;;) {
        if (++stream_index >= nb_streams)
        {
            if (codec_type == AVMEDIA_TYPE_SUBTITLE)
            {
                stream_index = -1;
                this->last_subtitle_stream = -1;
                goto the_end;
            }
            if (start_index == -1)
                return;
            stream_index = 0;
        }
        if (stream_index == start_index)
            return;
        st = this->pFormatCtx->streams[p ? p->stream_index[stream_index] : stream_index];
        if (st->codecpar->codec_type == codec_type) {
            /* check that parameters are OK */
            switch (codec_type) {
            case AVMEDIA_TYPE_AUDIO:
                if (st->codecpar->sample_rate != 0 &&
                    st->codecpar->channels != 0)
                    goto the_end;
                break;
            case AVMEDIA_TYPE_VIDEO:
            case AVMEDIA_TYPE_SUBTITLE:
                goto the_end;
            default:
                break;
            }
        }
    }
the_end:
    if (p && stream_index != -1)
        stream_index = p->stream_index[stream_index];
    av_log(nullptr, AV_LOG_INFO, "Switch %s stream from #%d to #%d\n", av_get_media_type_string(static_cast<AVMediaType>(codec_type)), old_index, stream_index);

    stream_component_close(old_index);
    stream_component_open(stream_index);
}

void FMediaPlayer::OnToggleFullScreen()
{
    is_full_screen = !is_full_screen;
    SDL_SetWindowFullscreen(pWindow, is_full_screen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    this->force_refresh = true;
}

void FMediaPlayer::OnToggleAudioDisplay()
{
    int next = static_cast<int>(this->eShow_mode);
    do {
        next = (next + 1) % int(FMediaPlayer::EShowMode::SHOW_MODE_NB);
    } while (next != int(this->eShow_mode) && (next == int(FMediaPlayer::EShowMode::SHOW_MODE_VIDEO) && !this->pVideoStream || next != int(FMediaPlayer::EShowMode::SHOW_MODE_VIDEO) && !this->pAudioStream));
    if (int(this->eShow_mode) != next) {
        this->force_refresh = true;
        this->eShow_mode = FMediaPlayer::EShowMode(next);
    }
}

void FMediaPlayer::RegisterRenderCallbck(const std::function<void(uint8_t ** buffer, int width, int height)>& renderFunctor)
{
    m_Rendercallback = renderFunctor;
}

void FMediaPlayer::refresh_loop_wait_event(SDL_Event& event)
{
    double remaining_time = 0.0;
#ifdef SDL_MAIN_HANDLED
    SDL_PumpEvents();
    while (!SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT))
    {
#else
;
#endif
        if (!cursor_hidden && av_gettime_relative() - cursor_last_shown > CURSOR_HIDE_DELAY)
        {
            SDL_ShowCursor(0);
            cursor_hidden = true;
        }
        if (remaining_time > 0.0)
            std::this_thread::sleep_for(std::chrono::microseconds(int64_t(remaining_time * 1000000.0)));
        remaining_time = REFRESH_RATE;
        if (this->eShow_mode != FMediaPlayer::EShowMode::SHOW_MODE_NONE && (!this->paused || this->force_refresh))
            video_refresh(remaining_time);
#ifdef SDL_MAIN_HANDLED
        //SDL_PumpEvents();
    }
#endif
}

void FMediaPlayer::OnSeekChapter(int incr)
{
    int64_t pos = int64_t(get_master_clock() * AV_TIME_BASE);
    unsigned int i;

    if (!pFormatCtx->nb_chapters)
        return;

    /* find the current chapter */
    for (i = 0; i < this->pFormatCtx->nb_chapters; i++) {
        AVChapter* ch = this->pFormatCtx->chapters[i];
        if (av_compare_ts(pos, AVRational{ 1, AV_TIME_BASE }, ch->start, ch->time_base) < 0) {
            i--;
            break;
        }
    }

    i += incr;
    i = FFMAX(i, 0);
    if (i >= this->pFormatCtx->nb_chapters)
        return;

    av_log(nullptr, AV_LOG_VERBOSE, "Seeking to chapter %d.\n", i);
    OnStreamSeek(av_rescale_q(this->pFormatCtx->chapters[i]->start, this->pFormatCtx->chapters[i]->time_base, AVRational{ 1, AV_TIME_BASE }), 0, false);
}

bool FMediaPlayer::OnTick()
{
    SDL_Event event;
    double incr, pos;

    double x = 0;
    refresh_loop_wait_event(event);
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
        {
            this->OnExit();
            return false;
        }

        // If we don't yet have a window, skip all key events, because read_thread might still be initializing...
        if (!this->rect.w)
            return true;
        switch (event.key.keysym.sym)
        {
        case SDLK_f:
            OnToggleFullScreen();
            break;
        case SDLK_p:
        case SDLK_SPACE:
            OnTogglePause();
            break;
        case SDLK_m:
            OnToggleMute();
            break;
        case SDLK_KP_MULTIPLY:
        case SDLK_0:
            OnUpdateVolume(1, SDL_VOLUME_STEP);
            break;
        case SDLK_KP_DIVIDE:
        case SDLK_9:
            OnUpdateVolume(-1, SDL_VOLUME_STEP);
            break;
        case SDLK_s: // S: Step to next frame
            OnStepToNextFrame();
            break;
        case SDLK_a:
            OnStreamCycleChannel(AVMEDIA_TYPE_AUDIO);
            break;
        case SDLK_v:
            OnStreamCycleChannel(AVMEDIA_TYPE_VIDEO);
            break;
        case SDLK_c:
            OnStreamCycleChannel(AVMEDIA_TYPE_VIDEO);
            OnStreamCycleChannel(AVMEDIA_TYPE_AUDIO);
            OnStreamCycleChannel(AVMEDIA_TYPE_SUBTITLE);
            break;
        case SDLK_t:
            OnStreamCycleChannel(AVMEDIA_TYPE_SUBTITLE);
            break;
        case SDLK_w:
#if CONFIG_AVFILTER
            if (this->eShow_mode == EShowMode::SHOW_MODE_VIDEO && this->vfilter_idx < nb_vfilters - 1) {
                if (++this->vfilter_idx >= nb_vfilters)
                    this->vfilter_idx = 0;
            }
            else {
                this->vfilter_idx = 0;
                this->OnToggleAudioDisplay();
            }
#else
            OnToggleAudioDisplay();
#endif
            break;
        case SDLK_PAGEUP:
            if (this->pFormatCtx->nb_chapters <= 1) {
                incr = 600.0;
                goto do_seek;
            }
            OnSeekChapter(1);
            break;
        case SDLK_PAGEDOWN:
            if (this->pFormatCtx->nb_chapters <= 1) {
                incr = -600.0;
                goto do_seek;
            }
            OnSeekChapter(-1);
            break;
        case SDLK_LEFT:
            incr = seek_interval ? -seek_interval : -10.0;
            goto do_seek;
        case SDLK_RIGHT:
            incr = seek_interval ? seek_interval : 10.0;
            goto do_seek;
        case SDLK_UP:
            incr = 60.0;
            goto do_seek;
        case SDLK_DOWN:
            incr = -60.0;
        do_seek:
            if (seek_by_bytes) {
                pos = -1;
                if (pos < 0 && this->video_stream >= 0)
                    pos = this->pictq.LastPos();
                if (pos < 0 && this->audio_stream >= 0)
                    pos = this->sampq.LastPos();
                if (pos < 0)
                    pos = avio_tell(this->pFormatCtx->pb);
                if (this->pFormatCtx->bit_rate)
                    incr *= this->pFormatCtx->bit_rate / 8.0;
                else
                    incr *= 180000.0;
                pos += incr;
                OnStreamSeek(pos, incr, true);
            }
            else {
                pos = get_master_clock();
                if (isnan(pos))
                    pos = (double)this->seek_pos / AV_TIME_BASE;
                pos += incr;
                if (this->pFormatCtx->start_time != AV_NOPTS_VALUE && pos < this->pFormatCtx->start_time / (double)AV_TIME_BASE)
                    pos = this->pFormatCtx->start_time / (double)AV_TIME_BASE;
                OnStreamSeek((int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), false);
            }
            break;
        default:
            break;
            }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
            static int64_t last_mouse_left_click = 0;
            if (av_gettime_relative() - last_mouse_left_click <= 500000) {
                OnToggleFullScreen();
                last_mouse_left_click = 0;
            }
            else {
                last_mouse_left_click = av_gettime_relative();
            }
        }
    case SDL_MOUSEMOTION:
        if (cursor_hidden) {
            SDL_ShowCursor(1);
            cursor_hidden = false;
        }
        cursor_last_shown = av_gettime_relative();
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button != SDL_BUTTON_RIGHT)
                break;
            x = event.button.x;
        }
        else {
            if (!(event.motion.state & SDL_BUTTON_RMASK))
                break;
            x = event.motion.x;
        }
        if (seek_by_bytes || this->pFormatCtx->duration <= 0) {
            uint64_t size = avio_size(this->pFormatCtx->pb);
            OnStreamSeek(size * x / this->rect.w, 0, true);
        }
        else {
            const int tns = this->pFormatCtx->duration / 1000000LL;
            const int thh = tns / 3600;
            const int tmm = (tns % 3600) / 60;
            const int tss = (tns % 60);
            const int frac = x / this->rect.w;
            const int ns = frac * tns;
            const int hh = ns / 3600;
            const int mm = (ns % 3600) / 60;
            const int ss = (ns % 60);
            av_log(nullptr, AV_LOG_INFO, "Seek to %2.0f%% (%2d:%02d:%02d) of total duration (%2d:%02d:%02d)       \n", double(frac * 100),
                hh, mm, ss, thh, tmm, tss);
            int64_t ts = frac * this->pFormatCtx->duration;
            if (this->pFormatCtx->start_time != AV_NOPTS_VALUE)
                ts += this->pFormatCtx->start_time;
            OnStreamSeek(ts, 0, false);
        }
        break;
    case SDL_WINDOWEVENT:
        switch (event.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            screen_width = this->rect.w = event.window.data1;
            screen_height = this->rect.h = event.window.data2;
            if (this->vis_texture) {
                SDL_DestroyTexture(this->vis_texture);
                this->vis_texture = nullptr;
            }
        case SDL_WINDOWEVENT_EXPOSED:
            this->force_refresh = true;
        }
        break;
    case SDL_QUIT:
    case FF_QUIT_EVENT:
        return false;
    default:
        break;
        }

        return true;
}
