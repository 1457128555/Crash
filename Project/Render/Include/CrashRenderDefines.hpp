#pragma once

#ifdef _WIN32
    #ifdef CrashRender_EXPORTS
        #define CRASH_RENDER_API __declspec(dllexport)
    #else
        #define CRASH_RENDER_API __declspec(dllimport)
    #endif
#else
    #define CRASH_RENDER_API
#endif