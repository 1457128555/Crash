#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashSingleton.hpp"

namespace Crash
{
    class ShaderProgram;
    class UniformBuffer;
    class Renderable;

    class CRASH_ENGINE_API Renderer : public Singleton<Renderer>
    {
    public:
        Renderer();
        ~Renderer();

        void bindUniformBlock(ShaderProgram* program);
        void updateCommonUniforms();

        void renderSingleObject(Renderable* renderable);

    private:
        UniformBuffer* mCommonUniformBuffer = nullptr;
    };
}