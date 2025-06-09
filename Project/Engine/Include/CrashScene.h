#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashCamera.h"
#include "CrashLight.h"

#include <string>

namespace Crash
{
    class CRASH_ENGINE_API Scene
    {
    public:
        explicit Scene(const std::string& name);
        virtual ~Scene() ;

        virtual void initialize();
        virtual void shutdown();

        virtual void update(float deltaTime);
        virtual void renderScene();

        Camera&    getCamera()                  { return mCamera; }
        DirLight&  getDirLight()                { return mDirLight; }
        SpotLight& getSpotLight()               { return mSpotLight; }

    protected:
        const std::string   mName;

        Camera              mCamera;
        DirLight            mDirLight;
        bool                mRenderDirLight = true; 
        
        SpotLight           mSpotLight;
    };
}