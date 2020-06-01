# Simple-FFmplay-console

# 使用方法如下:

    方式一:本地媒体文件路径

    方式二:直播地址如下:

        湖南卫视 rtmp://58.200.131.2:1935/livetv/hunantv
        广西卫视 ：rtmp://58.200.131.2:1935/livetv/gxtv
        广东卫视：rtmp://58.200.131.2:1935/livetv/gdtv
        东方卫视：rtmp://58.200.131.2:1935/livetv/dftv

    方式三:获取播放设备
        ./ffmpeg -f dshow -list_devices true -i dummy

        本机播放设备: "Logitech HD Webcam C270"

        捕获camera图像信息并播放
        -f dshow -i video="Logitech HD Webcam C270"

