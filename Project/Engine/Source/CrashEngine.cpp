
#include "CrashPCH.h"
#include "CrashEngine.h"

#include "CrashTexMgr.h"
#include "CrashInputMgr.h"
#include "CrashRenderSystem.h"

#include "CrashScene.h"
#include "CrashCamera.h"

namespace Crash
{
    Engine::Engine(const EngineConfig& config)
        : mConfig(config)
    {   
        new LogManager(config.outputFunc); 
        new TexMgr;
        new InputMgr;

        std::string msg = "Engine initialized\n";
        msg += "width :\t"      + std::to_string(config.windowWidth)  + '\n';
        msg += "height:\t"      + std::to_string(config.windowHeight) + '\n';
        msg += "asyncRender:\t" + std::to_string(config.asyncRender)  + '\n';

        LogManager::Instance()->log(msg);

        new RenderSystem(config.procAddress, config.asyncRender);
        RenderSystem::Instance()->setReverseZ(mReverseZ);

        if(mConfig.asyncRender)
        {
            mRenderThread = std::thread([config]()
            {
                LogManager::Instance()->log("Render thread started");

                config.initRenderContext();
                RenderSystem::Instance()->init();

                while (!Engine::Instance()->getClose())
                {
                    if(RenderSystem::Instance()->executeCommand())
                        config.swapBuffer();
                }

                LogManager::Instance()->log("Render thread stopped");
                RenderSystem::Instance()->uninit();
                config.destroyRenderContext();
            });
        }
        else
        {
            config.initRenderContext();
            RenderSystem::Instance()->init();
        }
    }

    Engine::~Engine()
    {
        LogManager::Instance()->log("Engine destroyed");
        setScene(nullptr);
        
        // Wait for the render thread to finish if it is running
        mClose = true;
        if(mConfig.asyncRender)
        {
            if(mRenderThread.joinable())
                mRenderThread.join();
        }
        else
        {
            RenderSystem::Instance()->uninit();
            mConfig.destroyRenderContext();      
        }
            
        delete TexMgr::Instance();
        delete InputMgr::Instance();
        delete RenderSystem::Instance();
        delete LogManager::Instance();
    }

    void Engine::update(float deltaTime)
    {
        if (mScene)
            mScene->update(deltaTime);
    
        if(mControlCamera)
            mControlCamera->update(deltaTime);  

        InputMgr::Instance()->update();
    }

    void Engine::renderFrame()
    {
        RenderSystem::Instance()->frameBegin();
        RenderSystem::Instance()->setViewport({ 0, 0, mConfig.windowWidth, mConfig.windowHeight });

        if(mScene)
            mScene->renderScene();
        
        RenderSystem::Instance()->frameEnd();
    }

    void Engine::renderFinish()
    {
        if(mConfig.asyncRender)
            RenderSystem::Instance()->submitCommand();
        else
            mConfig.swapBuffer();
    }

    void Engine::setScene(Scene* scene)
    {
       if(mScene)
       {
            mScene->shutdown();  
       }

        mScene = scene;
        if(mScene)
        {
            mScene->initialize();
        }
    }
} // namespace Crash