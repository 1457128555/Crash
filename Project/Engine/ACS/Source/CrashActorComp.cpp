#include "CrashPCH.h"
#include "CrashActorComp.h"

namespace Crash
{
    ActorComp::ActorComp(ActorCompType _type, const std::string& _name)
        : mType(_type)
        , mName(_name.empty() ? (mName + std::to_string(sCRef)) : _name)
    {
       

    }
    
    ActorComp::~ActorComp()
    {
        
    }
}