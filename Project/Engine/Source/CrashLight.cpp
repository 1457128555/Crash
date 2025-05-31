#include "CrashPCH.h"
#include "CrashLight.h"

#include "CrashRenderSystem.h"

namespace Crash
{
    void Light::apply(const ShaderProgram* program) const
    {
        RenderSystem::Instance()->setUniform4f(program, getType() + ".ambient",  mAmbient);
        RenderSystem::Instance()->setUniform4f(program, getType() + ".diffuse",  mDiffuse);
        RenderSystem::Instance()->setUniform4f(program, getType() + ".specular", mSpecular);
    }

    void PointLight::apply(const ShaderProgram* program) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        Light::apply(program);

        RenderSystem::Instance()->setUniform4f(program, "uPointLight.position",      mPosition);
        RenderSystem::Instance()->setUniform4f(program, "uPointLight.attenuation",   mAttenuation);
    }

    void DirLight::apply(const ShaderProgram* program) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        Light::apply(program);

        RenderSystem::Instance()->setUniform4f(program, "uDirLight.direction", mDirection);
    }

    void SpotLight::apply(const ShaderProgram* program) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        Light::apply(program);

        RenderSystem::Instance()->setUniform4f(program, "uSpotLight.position",   mPosition);
        RenderSystem::Instance()->setUniform4f(program, "uSpotLight.direction",  mDirection);
        RenderSystem::Instance()->setUniform4f(program, "uSpotLight.attenuation", mAttenuation);
        RenderSystem::Instance()->setUniform4f(program, "uSpotLight.cutOff",      mCutOff);
    }
}