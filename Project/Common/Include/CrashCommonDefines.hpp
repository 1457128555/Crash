#pragma once

#ifdef _WIN32
    #ifdef CrashCommon_EXPORTS
        #define CRASH_COMMON_API __declspec(dllexport)
    #else
        #define CRASH_COMMON_API __declspec(dllimport)
    #endif
#else
    #define CRASH_COMMON_API
#endif