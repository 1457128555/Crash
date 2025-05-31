#include "CrashPCH.h"
#include "CrashMaterial.h"

#include "CrashRenderSystem.h"

namespace Crash
{
    
    void Material::apply(const ShaderProgram* program) const
    {
        assert(program != nullptr && "ShaderProgram is null!");
        if (!program) return;

        RenderSystem::Instance()->setUniform4f(program, "uMaterial.ambient", mAmbient);
        RenderSystem::Instance()->setUniform4f(program, "uMaterial.diffuse", mDiffuse);
        RenderSystem::Instance()->setUniform4f(program, "uMaterial.specular", mSpecular);

        if(mDiffuseTex)
        {
            RenderSystem::Instance()->setUniform1i(program, "uDiffuseTex", 0);
            RenderSystem::Instance()->activateTextureUnit(0);
            RenderSystem::Instance()->bindTexture(mDiffuseTex);
        }

        if(mSpecularTex)
        {
            RenderSystem::Instance()->setUniform1i(program, "uSpecularTex", 1);
            RenderSystem::Instance()->activateTextureUnit(1);
            RenderSystem::Instance()->bindTexture(mSpecularTex);
        }
    }
}