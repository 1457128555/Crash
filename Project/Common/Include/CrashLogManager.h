#pragma once

#include "CrashSingleton.hpp"
#include "CrashCommonDefines.hpp"
#include <string>

namespace Crash
{
    class CRASH_COMMON_API LogManager : public Singleton<LogManager>
    {
    public:
        LogManager(void(*func)(const std::string&));

        void log(const std::string& message);

    private:
        void(*mOutputFunc)(const std::string&) = nullptr;
    };
}