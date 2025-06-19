#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashActorChild.h"

#include <string>

namespace Crash
{    
    enum class ActorCompType : unsigned int
    {
        None = 0,  
    };

    class CRASH_ENGINE_API ActorComp : public ActorChild
    {
    public:
        ActorComp(ActorCompType _type, const std::string& _name = "");
        virtual ~ActorComp() override;

        ActorCompType       getType()       { return mType; };
        const std::string&  getName()       { return mName; };
        
    protected:
        ActorCompType   mType   = ActorCompType::None;
        std::string     mName   = "DefaultActorComp";

    protected:
        inline static unsigned int sCRef = 0u;
    };
}