#include "CrashPCH.h"
#include "CrashInputMgr.h"

namespace Crash
{
    void InputMgr::onKeyDown(KeyCode key)
    {
        mKeyDownSet.insert(key);
    }

    void InputMgr::onKeyUp(KeyCode key)
    {
        mKeyDownSet.erase(key);
    }

    bool InputMgr::isKeyDown(KeyCode key) const
    {
        return mKeyDownSet.find(key) != mKeyDownSet.end();
    }

    void InputMgr::setMouseDelta(double dx, double dy)
    {
        mMouseDeltaX = dx;
        mMouseDeltaY = dy;
    }

    void InputMgr::getMouseDelta(double& dx, double& dy) const
    {
        dx = mMouseDeltaX;
        dy = mMouseDeltaY;
    }

    void InputMgr::update()
    {
        mMouseDeltaX = 0.0;
        mMouseDeltaY = 0.0;
        mKeyDownSet.clear();
    }
}