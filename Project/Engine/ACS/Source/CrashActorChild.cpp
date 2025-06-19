#include "CrashPCH.h"
#include "CrashActorChild.h"

namespace Crash
{
    ActorChild::~ActorChild()
    {
        assert(false == isAttachActor() && "need dis attach");
    }

    void ActorChild::attachActorImpl()
    {
        assert(false && "need override");
    }

    void ActorChild::disattachActorImpl()
    {
        assert(false && "need override");
    }

    void ActorChild::attachActor(Actor* _parent)
    {
        disattachActor();

        mParent = _parent;
        attachActorImpl();
    }

    void ActorChild::disattachActor()
    {
        if(!mParent) 
            return;

        disattachActorImpl();
        mParent = nullptr;
    }
}