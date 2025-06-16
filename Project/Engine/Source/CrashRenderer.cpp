#include "CrashPCH.h"
#include "CrashRenderer.h"

#include "CrashEngine.h"
#include "CrashScene.h"
#include "CrashCamera.h"
#include "CrashRenderSystem.h"
#include "CrashRenderBuffer.h"

namespace Crash
{
    namespace
    {
        struct _CommonUBO
        {
            glm::vec4 viewPos       =  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            glm::mat4 view          =  glm::mat4(1.0f);
            glm::mat4 projection    =  glm::mat4(1.0f);
        };
    }

    Renderer::Renderer()
    {
        mCommonUniformBuffer = RenderSystem::Instance()->createUniformBuffer();
        RenderSystem::Instance()->setBufferData(mCommonUniformBuffer, nullptr, sizeof(_CommonUBO));
        RenderSystem::Instance()->setBindBufferBase(mCommonUniformBuffer, 0);
    }

    Renderer::~Renderer()
    {
        RenderSystem::Instance()->destroyUniformBuffer(mCommonUniformBuffer);
        mCommonUniformBuffer = nullptr; 
    }

    void Renderer::bindUniformBlock(ShaderProgram* program)
    {
        assert(program != nullptr && "ShaderProgram cannot be null!");
        RenderSystem::Instance()->setUniformBlockBinding(program, "CommonUBO", 0);
    }

    void Renderer::updateCommonUniforms()
    {
        
        _CommonUBO commonUBO;

        // Update common uniforms like view, projection, etc.
        const Scene* scene = Engine::Instance()->getScene();
        if (scene)
        {
            const Camera& camera = scene->getCamera();

            commonUBO.viewPos = glm::vec4(camera.getPosition(), 1.0f);

            commonUBO.view = camera.getViewMat();
            commonUBO.projection = camera.getProjectionMat(Engine::Instance()->getAspect());
        }

        RenderSystem::Instance()->setBufferSubData(mCommonUniformBuffer, 0u, &commonUBO, sizeof(_CommonUBO));
    }
} // namespace Crash

