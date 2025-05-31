#pragma once

#include "CrashEngineDefines.hpp"

#include <string>

namespace Crash
{
    class CRASH_ENGINE_API Scene
    {
    public:
        explicit Scene(const std::string& name): mName(name){};

        virtual ~Scene()                        = default;

        virtual void initialize()               = 0;
        virtual void update(float deltaTime)    = 0;

        virtual void renderScene()              = 0;
        
        virtual void shutdown()                 = 0;

    protected:
        const std::string mName;
    };
}