 #include "CrashPCH.h"
 #include "HelloSkyCube05.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashCamera.h"
#include "CrashFileSystem.h"
#include "CrashTexMgr.h"
#include "CrashBasicGeometry.h"

using namespace Crash;

namespace
{
    ShaderProgram*      gSkyCubeShader    = nullptr;
    ShaderProgram*      gMirrorCubeShader = nullptr;

    VertexArrayObject*  gSkyCubeVAO       = nullptr;
    VertexBuffer*       gSkyCubeVBO       = nullptr;

    std::shared_ptr<Texture> gSkyTexture;
}

void HelloSkyCube05::initialize()       
{
    Scene::initialize();
    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    std::array<std::string, 6> skyboxFaces = {
        "skybox/right.jpg",  
        "skybox/left.jpg",   
        "skybox/top.jpg",    
        "skybox/bottom.jpg", 
        "skybox/front.jpg",  
        "skybox/back.jpg",   
    };

    gSkyTexture = TexMgr::Instance()->createCubeMapTexture("skybox",skyboxFaces );

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloSkyCube05_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloSkyCube05_PS.txt");
        gSkyCubeShader = RenderSystem::Instance()->createShaderProgram("HelloSkyCube05", vsCode, psCode);
    }

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloSkyCube05_Mirror_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloSkyCube05_Mirror_PS.txt");
        gMirrorCubeShader = RenderSystem::Instance()->createShaderProgram("HelloSkyCube05_Mirror", vsCode, psCode);
    }

    {
        auto dataType = BasicGeometry::ComFlag({
        BasicGeometry::DataType::Vertex, 
        BasicGeometry::DataType::Normal});
        
        std::vector<float> vertices;
        BasicGeometry::Cube(dataType, vertices);

        gSkyCubeVAO = RenderSystem::Instance()->createVertexArray();
        gSkyCubeVBO = RenderSystem::Instance()->createBuffer();
        RenderSystem::Instance()->setBufferData(gSkyCubeVBO, vertices.data(), sizeof(vertices[0]) * vertices.size());
        RenderSystem::Instance()->addBufferToVertexArray(gSkyCubeVAO, gSkyCubeVBO, 0, 3, sizeof(float) * 6, (const void*)0);
        RenderSystem::Instance()->addBufferToVertexArray(gSkyCubeVAO, gSkyCubeVBO, 1, 3, sizeof(float) * 6, (const void*)(sizeof(float) * 3));
        RenderSystem::Instance()->unbindVertexArray();
    }

}

void HelloSkyCube05::shutdown()               
{
    Scene::shutdown();

    gSkyTexture.reset();

    RenderSystem::Instance()->destroyBuffer(gSkyCubeVBO);
    RenderSystem::Instance()->destroyVertexArray(gSkyCubeVAO);
    RenderSystem::Instance()->destroyShaderProgram(gSkyCubeShader);
    RenderSystem::Instance()->destroyShaderProgram(gMirrorCubeShader);
}

void HelloSkyCube05::renderScene()
{
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    RenderSystem::Instance()->setCullFaceEnable(false);
    RenderSystem::Instance()->setDepthFunc(RenderProtocol::CompareFunc::LessEqual);

    //  Render Mirror
    {
        RenderSystem::Instance()->bindVertexArray(gSkyCubeVAO);
        RenderSystem::Instance()->bindShaderProgram(gMirrorCubeShader);

        RenderSystem::Instance()->setUniform1i(gMirrorCubeShader, "uCubeMap", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gSkyTexture.get());

        glm::mat4 model = glm::mat4(1.f);
        RenderSystem::Instance()->setUniformMatrix4fv(gMirrorCubeShader, "uModel", model);
        RenderSystem::Instance()->setUniformMatrix4fv(gMirrorCubeShader, "uView", mCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gMirrorCubeShader, "uProjection", mCamera.getProjectionMat(Engine::Instance()->getAspect()));

        RenderSystem::Instance()->setUniform4f(gMirrorCubeShader, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));

        RenderSystem::Instance()->drawArray(RenderProtocol::DrawMode::Triangles, 0, 36);
    }

    {
        RenderSystem::Instance()->bindVertexArray(gSkyCubeVAO);
        RenderSystem::Instance()->bindShaderProgram(gSkyCubeShader);

        RenderSystem::Instance()->setUniform1i(gSkyCubeShader, "uCubeMap", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gSkyTexture.get());

        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, mCamera.getPosition());
        RenderSystem::Instance()->setUniformMatrix4fv(gSkyCubeShader, "uModel", model);
        RenderSystem::Instance()->setUniformMatrix4fv(gSkyCubeShader, "uView", mCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gSkyCubeShader, "uProjection", mCamera.getProjectionMat(Engine::Instance()->getAspect()));

        RenderSystem::Instance()->drawArray(RenderProtocol::DrawMode::Triangles, 0, 36);
    }


}