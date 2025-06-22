#pragma once

#include "CrashEngineDefines.hpp"

namespace Crash
{
    class Actor;

    class CRASH_ENGINE_API ActorChild
    {
    public:
        ActorChild() = default;
        virtual ~ActorChild();

        void attachActor(Actor* _parent);
        void disattachActor();

        bool    isAttachActor()     const   { return mParent; };
        Actor*  getParent()         const   { return mParent; };

    protected:
        virtual void attachActorImpl();
        virtual void disattachActorImpl();

    protected:
        Actor* mParent = nullptr;
    };
} // namespace Crash
