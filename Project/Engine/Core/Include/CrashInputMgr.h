#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashSingleton.hpp"

#include <unordered_set>

namespace Crash
{
    class CRASH_ENGINE_API InputMgr : public Singleton<InputMgr>
    {
    public:
        enum class KeyCode : unsigned int
        {
            KEY_W, KEY_A, KEY_S, KEY_D,
            MOUSE_BUTTON_RIGHT,
        };

        void onKeyDown(KeyCode key);
        void onKeyUp(KeyCode key);
        bool isKeyDown(KeyCode key) const;

        void setMouseDelta(double dx, double dy);
        void getMouseDelta(double& dx, double& dy) const;

        void update(); 

    private:
        double mMouseDeltaX = 0.0, mMouseDeltaY = 0.0;
        std::unordered_set<KeyCode> mKeyDownSet;
    };
}