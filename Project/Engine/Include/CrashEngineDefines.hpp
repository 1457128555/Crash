#pragma once

#ifdef _WIN32
    #ifdef CrashEngine_EXPORTS
        #define CRASH_ENGINE_API __declspec(dllexport)
    #else
        #define CRASH_ENGINE_API __declspec(dllimport)
    #endif
#else
    #define CRASH_ENGINE_API
#endif