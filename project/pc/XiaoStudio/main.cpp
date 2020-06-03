#include "mainwindow.h"

#include <QApplication>
#include <iostream>

//#include "SDL_main.h"

#if defined(Q_OS_WIN32)
    #include "qt_windows.h"
    #include "WinUser.h"
    //#include "Iphlpapi.h"
    #include "dbghelp.h"

    #pragma auto_inline (off)

    //#pragma comment(lib,"Iphlpapi")
    #pragma comment(lib, "DbgHelp" )
    #pragma comment(lib, "user32")

/* 生成dump文件 */
static int GenerateMiniDump(PEXCEPTION_POINTERS exception_Pointers)
{
    TCHAR file_name[MAX_PATH] = { 0 };
    SYSTEMTIME local_time;
    GetLocalTime(&local_time);
    wsprintf(file_name, L"TestDeomo_crash-%04d%02d%02d-%02d-%02d-%02d.dmp",
             local_time.wYear, local_time.wMonth, local_time.wDay,
             local_time.wHour, local_time.wMinute, local_time.wSecond);
    HANDLE h_dump_file = CreateFile(file_name, GENERIC_READ | GENERIC_ALL, FILE_SHARE_READ, nullptr, OPEN_EXISTING, NULL, nullptr);

    MINIDUMP_EXCEPTION_INFORMATION exception_information;
    exception_information.ThreadId = GetCurrentThreadId();
    exception_information.ExceptionPointers = exception_Pointers;
    exception_information.ClientPointers = FALSE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), h_dump_file, MiniDumpNormal, &exception_information, nullptr, nullptr);

    CloseHandle(h_dump_file);

    return EXCEPTION_EXECUTE_HANDLER;
}

/* 异常处理函数 */
static LONG WINAPI handle_exception(LPEXCEPTION_POINTERS lpExceptionInfo)
{
    return GenerateMiniDump(lpExceptionInfo);
}

#endif

static void init_dynload(void)
{
#ifdef _WIN32
    /* Calling SetDllDirectory with the empty string (but not NULL) removes the
     * current working directory from the DLL search path as a security pre-caution. */
    SetDllDirectory(L"");
#endif
}


int main(int argc, char *argv[])
{
//#if defined(Q_OS_WIN32)
//    /* 内存泄漏检测 */
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

//    /* 程序崩溃生成dump文件 */
//    SetUnhandledExceptionFilter(handle_exception);
//#endif

//    std::cout << "Program Start..." << std::endl;

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    const int appRes = a.exec();

//    std::cout << "Program Ending..." << std::endl;

//    assert(_CrtCheckMemory());
//    return appRes;

    const std::string input_filename = "D:/File/Movie/Interstellar/Interstellar.mkv";

        if (input_filename.empty())
        {
            av_log(nullptr, AV_LOG_FATAL, "An input file must be specified\n");
            av_log(nullptr, AV_LOG_FATAL, "Use -h to get full help or, even better, run 'man %s'\n", "FFmplay");
            exit(1);
        }

        init_dynload();

        std::unique_ptr<FMediaPlayer> pPlayer1 = std::make_unique<FMediaPlayer>();
        if (pPlayer1 == nullptr)
        {
            av_log(nullptr, AV_LOG_FATAL, "Failed to Create FMediaPlayer\n");
            return -1;
        }

        signal(SIGINT, FMediaPlayer::sigterm_handler);  /* Interrupt (ANSI).    */
        signal(SIGTERM, FMediaPlayer::sigterm_handler); /* Termination (ANSI).  */

        if (!pPlayer1->OnStreamOpen(input_filename))
        {
            av_log(nullptr, AV_LOG_FATAL, "Failed to Open stream\n");
            return -1;
        }

        /* TODO #SEQ1 */
        for (;;)
        {
            if(!pPlayer1->OnTick())
                break;
        }

        /* never returns */
        return 0;
}

