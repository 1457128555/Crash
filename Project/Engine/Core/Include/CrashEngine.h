#pragma once

#include "CrashEngineDeclare.hpp"
#include "CrashEngineDefines.hpp"
#include "CrashSingleton.hpp"

#include <string>

namespace Crash
{
    struct EngineConfig
    {
        int windowWidth     = 800;
        int windowHeight    = 600;
        void(*outputFunc)(const std::string&) = nullptr;
        void* procAddress   = nullptr;

        bool asyncRender                = true;
        void(*initRenderContext)()      = []() {};
        void(*destroyRenderContext)()   = []() {};
        void(*swapBuffer)()             = nullptr;

        std::string assetPath = R"(../assets/)";
    };

    class Scene;
    class Camera;

    class CRASH_ENGINE_API Engine : public Singleton<Engine>
    {
    public:
        explicit Engine(const EngineConfig& config);
        ~Engine();
            
        void update(float deltaTime);
        
        void renderFrame();

        void renderFinish();

        void setScene(Scene* scene);
        Scene* getScene() const { return mScene; }

        void setExecuteTime(double time) { mExecutetime = time; }
        double getExecuteTime() const { return mExecutetime; }

        const std::string&  getAssetPath()  const { return mConfig.assetPath; }

        void setClose(bool close) { mClose = close; }
        bool getClose() const { return mClose; }

        float getAspect() const { return float(mConfig.windowWidth) / float(mConfig.windowHeight); };

        bool isReverseZ() const { return mReverseZ; }

        int getWidth()  const { return mConfig.windowWidth; }
        int getHeight() const { return mConfig.windowHeight; }

        const ActorSrdPtr& getRootActor()           { return mRootActor; };
        void setRootActor(const ActorSrdPtr& _ptr)  { mRootActor = _ptr; };

    private:
        const EngineConfig mConfig;
        
        bool    mClose = false;
        Scene*  mScene = nullptr;

        double  mExecutetime = 0.0f;

        std::thread mRenderThread;

        bool    mReverseZ = false;      // 反转深度 - 消除z-fighting问题


        //  ASC Begin
        ActorSrdPtr mRootActor;
    };
}