#include "CrashPCH.h"
#include "CrashScene.h"

#include "CrashEngine.h"
#include "CrashTexMgr.h"

namespace Crash
{
    Scene::Scene(const std::string& name): mName(name)
    {
        {
            mDirLight.setDirection({-0.2f, -1.0f, -0.3f});
            mDirLight.setAmbient({0.2f, 0.2f, 0.2f});
            mDirLight.setDiffuse({0.5f, 0.5f, 0.5f});
            mDirLight.setSpecular({1.0f, 1.0f, 1.0f});
        }
        {
            mSpotLight.setAmbient({0.2f, 0.2f, 0.2f});
            mSpotLight.setDiffuse({0.5f, 0.5f, 0.5f});
            mSpotLight.setSpecular({1.0f, 1.0f, 1.0f});
        }
    };

    Scene::~Scene()
    {

    }

    void Scene::initialize()
    {
        std::array<std::string, 6> skyboxFaces = {
        "skybox/right.jpg",  
        "skybox/left.jpg",   
        "skybox/top.jpg",    
        "skybox/bottom.jpg", 
        "skybox/front.jpg",  
        "skybox/back.jpg",   
        };

        mSkyCube = TexMgr::Instance()->createCubeMapTexture("skybox",skyboxFaces );
    }

    void Scene::shutdown()
    {
        mSkyCube.reset();
    }

    void Scene::update(float deltaTime)
    {
        mCamera.update(deltaTime);  
        mSpotLight.setPosition(mCamera.getPosition());
        mSpotLight.setDirection(mCamera.getFront());
    }

    void Scene::renderScene()
    {

    }
}