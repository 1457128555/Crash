#pragma once

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

        void setControl(Camera* camera){ mControlCamera = camera; };

        void setExecuteTime(double time) { mExecutetime = time; }
        double getExecuteTime() const { return mExecutetime; }

        const std::string&  getAssetPath()  const { return mConfig.assetPath; }

        void setClose(bool close) { mClose = close; }
        bool getClose() const { return mClose; }

        float getAspect() const { return float(mConfig.windowWidth) / float(mConfig.windowHeight); };
    private:
        const EngineConfig mConfig;
        
        bool    mClose = false;
        Scene*  mScene = nullptr;
        Camera* mControlCamera = nullptr;

        double  mExecutetime = 0.0f;

        std::thread mRenderThread;

    };
}