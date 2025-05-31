#include "CrashPCH.h"
#include "CrashLogManager.h"

namespace Crash
{
    LogManager::LogManager(void(*func)(const std::string&))
    {
        mOutputFunc = func;
    }

    void LogManager::log(const std::string& message)
    {
        if (mOutputFunc != nullptr)
        {
            mOutputFunc(message);
        }
    }
} // namespace Crash