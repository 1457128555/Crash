 #include "CrashPCH.h"
 #include "HelloAssimp03.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashMesh.h"
#include "CrashFileSystem.h"
#include "CrashBasicGeometry.h"

using namespace Crash;

Mesh*               gMesh               = nullptr;
ShaderProgram*      gModelShader        = nullptr;
ShaderProgram*      gModelOuterShader   = nullptr;
ShaderProgram*      gLightProgram       = nullptr;

std::array<PointLight, 4> gPointLight;

BasicGeometry::RenderPack gCubeRP;

void HelloAssimp03::renderScene()
 {
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

     //  Render Point Light
    // {
    //     RenderSystem::Instance()->bindVertexArray(gCubeRP.vao);
    //     RenderSystem::Instance()->bindShaderProgram(gLightProgram);

    //     glm::mat4 view = mCamera.getViewMat();
    //     RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uView", view);
        
    //     glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
    //     RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uProjection", projection);
        
    //     RenderSystem::Instance()->setUniform4f(gLightProgram, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));
        
    //     for(auto &&pointLight : gPointLight)
    //     {
    //         glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(pointLight.getPosition()));
    //         model = glm::scale(model, glm::vec3(0.1f)); 
    //         RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uModel", model);

    //         pointLight.apply(gLightProgram);

    //         RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
    //             36, RenderProtocol::DrawElementType::UnsignedInt, 0);
    //     }

    //     RenderSystem::Instance()->unbindShaderProgram();
    //     RenderSystem::Instance()->unbindVertexArray();
    // }

    // //  Render Dir Light
    // {
    //     RenderSystem::Instance()->bindVertexArray(gCubeRP.vao);
    //     RenderSystem::Instance()->bindShaderProgram(gLightProgram);

    //     glm::mat4 model = glm::translate(glm::mat4(1.f), -50.f * glm::vec3(mDirLight.getDirection()));
    //     model = glm::scale(model, glm::vec3(4.f)); 
    //     RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uModel", model);

    //     glm::mat4 view = mCamera.getViewMat();
    //     RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uView", view);
    
    //     glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
    //     RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uProjection", projection);

    //     RenderSystem::Instance()->setUniform4f(gLightProgram, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));

    //     mDirLight.apply(gLightProgram);

    //     RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
    //         36, RenderProtocol::DrawElementType::UnsignedInt, 0);

    //     RenderSystem::Instance()->unbindShaderProgram();
    //     RenderSystem::Instance()->unbindVertexArray();
    // }

    // //  Set Pass State
    // {
    //     RenderSystem::Instance()->setStencilEnable(true);
    //     RenderSystem::Instance()->setStencilFunc(RenderProtocol::CompareFunc::Always, 1, 0xFF);
    //     RenderSystem::Instance()->setStencilOp(RenderProtocol::OperateFunc::Replace, RenderProtocol::OperateFunc::Replace, RenderProtocol::OperateFunc::Replace);
    // }

    // //  Render Model
    // {
    //     RenderSystem::Instance()->bindShaderProgram(gModelShader);

    //     glm::mat4 model = glm::mat4(1.f);
    //     RenderSystem::Instance()->setUniformMatrix4fv(gModelShader, "uModel", model);

    //     glm::mat4 view = mCamera.getViewMat();
    //     RenderSystem::Instance()->setUniformMatrix4fv(gModelShader, "uView", view);
    
    //     glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
    //     RenderSystem::Instance()->setUniformMatrix4fv(gModelShader, "uProjection", projection);

    //     RenderSystem::Instance()->setUniform4f(gModelShader, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));

    //     for(int i = 0; i < 4; ++i)
    //         gPointLight[i].apply(gModelShader, i);
        
    //     mDirLight.apply(gModelShader);
    //     mSpotLight.apply(gModelShader);

    //     gModel->draw(gModelShader);

    //     RenderSystem::Instance()->unbindShaderProgram();
    // }

    // //  Set Pass State
    // {
    //     RenderSystem::Instance()->setDepthEnable(false);
    //     RenderSystem::Instance()->setStencilFunc(RenderProtocol::CompareFunc::NotEqual, 1, 0xFF);
    //     RenderSystem::Instance()->setStencilOp(RenderProtocol::OperateFunc::Keep, RenderProtocol::OperateFunc::Keep, RenderProtocol::OperateFunc::Keep);
    // }

    
    // //  Render Outer Model
    // {
    //     RenderSystem::Instance()->bindShaderProgram(gModelOuterShader);

    //     glm::mat4 model = glm::mat4(1.f);
    //     model = glm::scale(model, glm::vec3(1.01f));
    //     RenderSystem::Instance()->setUniformMatrix4fv(gModelOuterShader, "uModel", model);

    //     glm::mat4 view = mCamera.getViewMat();
    //     RenderSystem::Instance()->setUniformMatrix4fv(gModelOuterShader, "uView", view);
    
    //     glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
    //     RenderSystem::Instance()->setUniformMatrix4fv(gModelOuterShader, "uProjection", projection);

    //     gModel->draw(gModelOuterShader);

    //     RenderSystem::Instance()->unbindShaderProgram();
    // }

    // //  Reset Pass State
    // {
    //     RenderSystem::Instance()->setDepthEnable(true);
    // }
}

void HelloAssimp03::initialize()       
{
    Scene::initialize();

    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    gMesh = new Mesh("backpack/backpack.obj");

    // gModel = new Model("backpack/backpack.obj");

    // {
    //     const std::string vsCode = Crash::FileSystem::ReadShader("HelloAssimp03_VS.txt");
    //     const std::string psCode = Crash::FileSystem::ReadShader("HelloAssimp03_PS.txt");
    //     gModelShader = RenderSystem::Instance()->createShaderProgram("HelloAssimp03", vsCode, psCode);

    //     const std::string psOuterCode = Crash::FileSystem::ReadShader("HelloAssimpOuter03_PS.txt");
    //     gModelOuterShader = RenderSystem::Instance()->createShaderProgram("HelloAssimpOuter03", vsCode, psOuterCode);
    // }

    // {
    //     const std::string vsCode = Crash::FileSystem::ReadShader("PointLight_VS.txt");
    //     const std::string psCode = Crash::FileSystem::ReadShader("PointLight_PS.txt");
    //     gLightProgram = RenderSystem::Instance()->createShaderProgram("PointLight", vsCode, psCode);
    // }

    // {
    //     glm::vec3 pointLightPositions[] = {
    //         glm::vec3( 0.7f,  0.2f,  2.0f),
    //         glm::vec3( 2.3f, -3.3f, -4.0f),
    //         glm::vec3(-4.0f,  2.0f, -12.0f),
    //         glm::vec3( 0.0f,  0.0f, -3.0f)
    //     };

    //     //  Init Point Light
    //     for(int i = 0; i < gPointLight.size(); ++i)
    //     {
    //         gPointLight[i].setPosition(pointLightPositions[i]);
    //         gPointLight[i].setAmbient({0.2f, 0.2f, 0.2f});
    //         gPointLight[i].setDiffuse({0.5f, 0.5f, 0.5f});
    //         gPointLight[i].setSpecular({1.0f, 1.0f, 1.0f});
    //     }
    // }
    
    // gCubeRP = BasicGeometry::CreateCubeRP(BasicGeometry::ComFlag({
    //     BasicGeometry::DataType::Vertex, 
    //     BasicGeometry::DataType::Normal,
    //     BasicGeometry::DataType::TexCoord}));
}

void HelloAssimp03::shutdown()               
{
    Scene::shutdown();

    delete gMesh;
    gMesh = nullptr;

    // delete gModel;
    // gModel = nullptr;

    // RenderSystem::Instance()->destroyShaderProgram(gModelShader);
    // gModelShader = nullptr;

    // RenderSystem::Instance()->destroyShaderProgram(gModelOuterShader);
    // gModelOuterShader = nullptr;

    // RenderSystem::Instance()->destroyShaderProgram(gLightProgram);
    // gLightProgram = nullptr;

    // BasicGeometry::DestoryRenderPack(gCubeRP);
}
