#include "CrashPCH.h"
#include "CrashLight.h"

#include "CrashRenderSystem.h"

namespace Crash
{
    std::string Light::getUniformName(const int lightIndex) const
    {
        if(lightIndex == -1)
            return getType();
        else
            return getType() + "[" + std::to_string(lightIndex) + "]";
    }

    void Light::apply(const ShaderProgram* program, int lightIndex) const
    {
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".ambient",  mAmbient);
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".diffuse",  mDiffuse);
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".specular", mSpecular);
    }

    void PointLight::apply(const ShaderProgram* program, int lightIndex) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        Light::apply(program, lightIndex);

        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".position",    mPosition);
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".attenuation", mAttenuation);
    }

    void DirLight::apply(const ShaderProgram* program, int lightIndex) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        Light::apply(program, lightIndex);

        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".direction", mDirection);
    }

    void SpotLight::apply(const ShaderProgram* program, int lightIndex) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        Light::apply(program, lightIndex);

        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".position",   mPosition);
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".direction",  mDirection);
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".attenuation", mAttenuation);
        RenderSystem::Instance()->setUniform4f(program, getUniformName(lightIndex) + ".cutOff",      mCutOff);
    }
}