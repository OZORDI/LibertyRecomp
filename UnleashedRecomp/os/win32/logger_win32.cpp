#include <os/logger_detail.h>

#define FOREGROUND_WHITE  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)

HANDLE g_hStandardOutput = nullptr;

void os::logger::detail::Log(const std::string& str, detail::ELogType type, const char* func)
{
    if (!g_hStandardOutput)
        g_hStandardOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    switch (type)
    {
        case ELogType::Utility:
            SetConsoleTextAttribute(g_hStandardOutput, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;

        case ELogType::Warning:
            SetConsoleTextAttribute(g_hStandardOutput, FOREGROUND_YELLOW | FOREGROUND_INTENSITY);
            break;

        case ELogType::Error:
            SetConsoleTextAttribute(g_hStandardOutput, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;

        default:
            SetConsoleTextAttribute(g_hStandardOutput, FOREGROUND_WHITE);
            break;
    }

    if (func)
    {
        printf("[%s] %s\n", func, str.c_str());
    }
    else
    {
        printf("%s\n", str.c_str());
    }

    SetConsoleTextAttribute(g_hStandardOutput, FOREGROUND_WHITE);
}
