#pragma once

#include "memory"

namespace Crash
{
    class Actor;
    using ActorSrdPtr = std::shared_ptr<Actor>;

    class ActorComp;
    using ActorCompSrdPtr = std::shared_ptr<ActorComp>;
}
