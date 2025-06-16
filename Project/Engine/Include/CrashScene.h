#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashCamera.h"
#include "CrashLight.h"

#include <string>

namespace Crash
{
    class Texture;

    class CRASH_ENGINE_API Scene
    {
    public:
        explicit Scene(const std::string& name);
        virtual ~Scene() ;

        virtual void initialize();
        virtual void shutdown();

        virtual void update(float deltaTime);
        virtual void renderScene();

        const Camera&       getCamera()                  const { return mCamera; }
        const DirLight&     getDirLight()                const { return mDirLight; }
        const SpotLight&    getSpotLight()               const { return mSpotLight; }
        
        Camera&             getCamera()                        { return mCamera; }
        DirLight&           getDirLight()                      { return mDirLight; }
        SpotLight&          getSpotLight()                     { return mSpotLight; }
        
        const std::shared_ptr<Texture>& getSkyCube()     const      { return mSkyCube; }
        void setSkyCube(const std::shared_ptr<Texture>& skyCube)    { mSkyCube = skyCube; }
    protected:
        const std::string   mName;

        Camera              mCamera;
        DirLight            mDirLight;
        bool                mRenderDirLight = true; 
        
        SpotLight           mSpotLight;

        std::shared_ptr<Texture> mSkyCube;
    };
}