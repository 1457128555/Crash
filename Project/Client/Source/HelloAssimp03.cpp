 #include "CrashPCH.h"
 #include "HelloAssimp03.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashMesh.h"
#include "CrashFileSystem.h"
#include "CrashBasicGeometry.h"
#include "CrashEntity.h"

using namespace Crash;

Entity*                         gEntity             = nullptr;
std::shared_ptr<ShaderProgram>  gModelShader;
std::shared_ptr<ShaderProgram>  gModelOuterShader;

void HelloAssimp03::renderScene()
 {
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    //  Set Pass State
    {
        RenderSystem::Instance()->setStencilEnable(true);
        RenderSystem::Instance()->setStencilFunc(RenderProtocol::CompareFunc::Always, 1, 0xFF);
        RenderSystem::Instance()->setStencilOp(RenderProtocol::OperateFunc::Replace, RenderProtocol::OperateFunc::Replace, RenderProtocol::OperateFunc::Replace);
    }

    //  Render Model
    glm::mat4& mWorldMatrix = gEntity->getWorldMatrix();
    mWorldMatrix = glm::mat4(1.f);
    gEntity->setShaderProgram(gModelShader);
    gEntity->render();

    //  Set Pass State
    {
        RenderSystem::Instance()->setDepthEnable(false);
        RenderSystem::Instance()->setStencilFunc(RenderProtocol::CompareFunc::NotEqual, 1, 0xFF);
        RenderSystem::Instance()->setStencilOp(RenderProtocol::OperateFunc::Keep, RenderProtocol::OperateFunc::Keep, RenderProtocol::OperateFunc::Keep);
    }

    //  Render Outer Model
    mWorldMatrix = glm::scale(glm::mat4(1.f), glm::vec3(1.01f));
    gEntity->setShaderProgram(gModelOuterShader);
    gEntity->render();

    //  Reset Pass State
    {
        RenderSystem::Instance()->setDepthEnable(true);
    }
}

void HelloAssimp03::initialize()       
{
    Scene::initialize();

    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    gEntity = new Entity(MeshMgr::Instance()->createMesh("backpack/backpack.obj"));

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloAssimp03_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloAssimp03_PS.txt");
        gModelShader = std::shared_ptr<ShaderProgram>(
            RenderSystem::Instance()->createShaderProgram("HelloAssimp03", vsCode, psCode),
            [](ShaderProgram* program) {
                RenderSystem::Instance()->destroyShaderProgram(program);
            }
        ); 

        const std::string psOuterCode = Crash::FileSystem::ReadShader("HelloAssimpOuter03_PS.txt");
        gModelOuterShader = std::shared_ptr<ShaderProgram>(
            RenderSystem::Instance()->createShaderProgram("HelloAssimpOuter03", vsCode, psOuterCode),
            [](ShaderProgram* program) {
                RenderSystem::Instance()->destroyShaderProgram(program);
            }
        ); 
    }



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

    delete gEntity;
    gEntity = nullptr;

    gModelShader.reset();
    gModelOuterShader.reset();

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
