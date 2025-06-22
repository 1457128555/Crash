#include "CrashPCH.h"
#include "CrashActor.h"
#include "CrashActorComp.h"

namespace Crash
{
    Actor::Actor(const std::string& _name)
        : mName(_name.empty() ? (mName + std::to_string(sCRef)) : _name)
    {

    }

    Actor::~Actor()
    {
        rmvActorAll();
        rmvCompAll();
    }

    void Actor::addActor(const ActorSrdPtr& _ptr)
    {
        auto findIt = std::find(mActors.begin(), mActors.end(), _ptr);
        if(mActors.end() != findIt)
        {
            assert(false && "repeat add actor!");
            return;
        }

        mActors.push_back(_ptr);
        _ptr->attachActor(this);
    }

    void Actor::rmvActor(const ActorSrdPtr& _ptr)
    {
        auto findIt = std::find(mActors.begin(), mActors.end(), _ptr);
        if(mActors.end() == findIt)
        {
            assert(false && "can't find actor!");
            return;
        }

        _ptr->disattachActor();
        mActors.erase(findIt);
    }

    void Actor::rmvActorAll()
    {
        for(auto it = mActors.begin(); it != mActors.end();)
        {
            (*it)->disattachActor();
            it = mActors.erase(it);
        }
    }

    void Actor::addComp(const ActorCompSrdPtr& _ptr)
    {
        auto findIt = std::find(mActorComps.begin(), mActorComps.end(), _ptr);
        if(mActorComps.end() != findIt)
        {
            assert(false && "repeat add comp!");
            return;
        }

        mActorComps.push_back(_ptr);
        _ptr->attachActor(this);
    }

    void Actor::rmvComp(const ActorCompSrdPtr& _ptr)
    {
        auto findIt = std::find(mActorComps.begin(), mActorComps.end(), _ptr);
        if(mActorComps.end() == findIt)
        {
            assert(false && "can't find comp!");
            return;
        }

        _ptr->disattachActor();
        mActorComps.erase(findIt);
    }

    void Actor::rmvCompAll()
    {
        for(auto it = mActorComps.begin(); it != mActorComps.end();)
        {
            (*it)->disattachActor();
            it = mActorComps.erase(it);
        }
    }
}