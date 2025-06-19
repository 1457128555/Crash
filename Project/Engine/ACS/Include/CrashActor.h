#pragma once

#include "CrashEngineDeclare.hpp"
#include "CrashEngineDefines.hpp"
#include "CrashActorChild.h"

namespace Crash
{
    class CRASH_ENGINE_API Actor : public ActorChild
    {
    public:
        explicit Actor(const std::string& _name);
        virtual ~Actor() override;

        void addActor(const ActorSrdPtr& _ptr);
        void rmvActor(const ActorSrdPtr& _ptr);
        void rmvActorAll();

        void addComp(const ActorCompSrdPtr& _ptr);
        void rmvComp(const ActorCompSrdPtr& _ptr);
        void rmvCompAll();
        
        const std::vector<ActorSrdPtr>&     getActors()     { return mActors; };
        const std::vector<ActorCompSrdPtr>& getActorComps() { return mActorComps; };

        const std::string& getName() { return mName; };
        
    protected:
        std::string mName = "DefaultActor";

        std::vector<ActorSrdPtr>        mActors;
        std::vector<ActorCompSrdPtr>    mActorComps;
    
    protected:
        inline static unsigned int sCRef = 0u;
    };
}