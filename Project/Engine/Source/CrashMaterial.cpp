#include "CrashPCH.h"
#include "CrashMaterial.h"

#include "CrashRenderSystem.h"
#include "CrashEngine.h"
#include "CrashScene.h"

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
            RenderSystem::Instance()->bindTexture(mDiffuseTex.get());
        }

        if(mSpecularTex)
        {
            RenderSystem::Instance()->setUniform1i(program, "uSpecularTex", 1);
            RenderSystem::Instance()->activateTextureUnit(1);
            RenderSystem::Instance()->bindTexture(mSpecularTex.get());
        }

        Scene* scene = Engine::Instance()->getScene();
        if (scene)
        {
            RenderSystem::Instance()->setUniform1i(program, "uSkyCube", 2);
            RenderSystem::Instance()->activateTextureUnit(2);
            RenderSystem::Instance()->bindTexture(scene->getSkyCube().get());
        }
    }

    void Material::reset()
    {
        mAmbient    = {0.f, 0.f, 0.f, 1.f};
        mDiffuse    = {0.f, 0.f, 0.f, 1.f};
        mSpecular   = {0.f, 0.f, 0.f, 1.f}; // W:shininess

        mDiffuseTex.reset();
        mSpecularTex.reset();
    }
}