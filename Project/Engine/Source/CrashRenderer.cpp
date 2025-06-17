#include "CrashPCH.h"
#include "CrashRenderer.h"

#include "CrashEngine.h"
#include "CrashScene.h"
#include "CrashCamera.h"
#include "CrashRenderSystem.h"
#include "CrashRenderBuffer.h"
#include "CrashRenderable.h"
#include "CrashRenderOperation.h"
#include "CrashMaterial.h"

namespace Crash
{
    namespace
    {
        struct _CommonUBO
        {
            glm::vec4 viewPos           =  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            glm::mat4 view              =  glm::mat4(1.0f);
            glm::mat4 projection        =  glm::mat4(1.0f);

            //   Directional Light
            glm::vec4 dirLightDir       =  glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
            glm::vec4 dirLightAmbient   =  glm::vec4(0.0f,  0.0f, 0.0f, 0.0f);
            glm::vec4 dirLightDiffuse   =  glm::vec4(0.0f,  0.0f, 0.0f, 0.0f);
            glm::vec4 dirLightSpecular  =  glm::vec4(0.0f,  0.0f, 0.0f, 0.0f);
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

            const DirLight& dirLight    = scene->getDirLight();
            commonUBO.dirLightDir       = dirLight.getDirection();
            commonUBO.dirLightAmbient   = dirLight.getAmbient();
            commonUBO.dirLightDiffuse   = dirLight.getDiffuse();
            commonUBO.dirLightSpecular  = dirLight.getSpecular();
        }

        RenderSystem::Instance()->setBufferSubData(mCommonUniformBuffer, 0u, &commonUBO, sizeof(_CommonUBO));
    }
    
    void Renderer::renderSingleObject(Renderable* renderable)
    {
        assert(renderable != nullptr && "Renderable cannot be null!");

        RenderOperation* renderOp = renderable->getRenderOperation();

        RenderSystem::Instance()->setCullFaceEnable(renderOp->getCullFace());
        renderOp->bind();

        ShaderProgram* shaderProgram = renderable->getShaderProgram();
        RenderSystem::Instance()->bindShaderProgram(shaderProgram);
        bindUniformBlock(shaderProgram);

        Material* material = renderable->getMaterial();
        material->apply(shaderProgram);

        RenderSystem::Instance()->setUniformMatrix4fv(shaderProgram, "uModel", *renderable->getWorldMatrix());
        renderable->setUniformObject();

        if(renderOp->getRenderType() == RenderOperation::RenderType::Array)
            RenderSystem::Instance()->drawArray(renderOp->getDrawMode(), renderOp->getFirst(), renderOp->getCount());
        else if(renderOp->getRenderType() == RenderOperation::RenderType::Element)
            RenderSystem::Instance()->drawElements(renderOp->getDrawMode(), renderOp->getCount(), renderOp->getElementType(), renderOp->getIndices());
        else if(renderOp->getRenderType() == RenderOperation::RenderType::ArrayInstance)
            RenderSystem::Instance()->drawArraysInstanced(renderOp->getDrawMode(), renderOp->getFirst(), renderOp->getCount(), renderOp->getInstanceCount());
        else if(renderOp->getRenderType() == RenderOperation::RenderType::ElementInstance)
            RenderSystem::Instance()->drawElementsInstanced(renderOp->getDrawMode(), renderOp->getCount(), renderOp->getElementType(), renderOp->getIndices(), renderOp->getInstanceCount());

        // Unbind the shader program after rendering
        RenderSystem::Instance()->unbindShaderProgram();
        renderOp->unbind();
    }
} // namespace Crash

