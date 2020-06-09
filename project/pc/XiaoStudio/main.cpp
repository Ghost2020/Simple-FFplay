#include "mainwindow.h"

#include <QApplication>
#include <iostream>

#if defined(Q_OS_WIN32)
    #include "qt_windows.h"
    #include "WinUser.h"
    #include "dbghelp.h"

    #pragma auto_inline (off)

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
#elif defined(Q_OS_MACOS)
;
#elif defined(Q_OS_LINUX)
;
#endif


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN32)
    /* 内存泄漏检测 */
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    /* 程序崩溃生成dump文件 */
    SetUnhandledExceptionFilter(handle_exception);
#endif

    std::cout << "Program Start..." << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    const int appRes = a.exec();

    std::cout << "Program Ending..." << std::endl;

#if defined(Q_OS_WIN32)
    assert(_CrtCheckMemory());
#endif
    return appRes;
}

