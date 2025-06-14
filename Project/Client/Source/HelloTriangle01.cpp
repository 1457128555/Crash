#include "CrashPCH.h"
#include "HelloTriangle01.h"

#include "CrashRenderSystem.h"
#include "CrashFileSystem.h"
#include "CrashRenderBuffer.h"
#include "CrashTexture.h"
#include "CrashEngine.h"
#include "CrashTexMgr.h"

#include "CrashBasicGeometry.h"

using namespace Crash;

namespace
{
    ShaderProgram*      gShaderProgram      = nullptr;

    BasicGeometry::RenderPack gTriangleRP;

    std::shared_ptr<Texture> gWallTex;
    std::shared_ptr<Texture> gFaceTex;
}

HelloTriangle01::HelloTriangle01() : Scene("HelloTriangle01")
{
    
}

void HelloTriangle01::renderScene()
{
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    RenderSystem::Instance()->bindVertexArray(gTriangleRP.vao);
    RenderSystem::Instance()->bindShaderProgram(gShaderProgram);

    RenderSystem::Instance()->activateTextureUnit(0);
    RenderSystem::Instance()->bindTexture(gWallTex.get());
    RenderSystem::Instance()->setUniform1i(gShaderProgram, "texture1", 0);

    RenderSystem::Instance()->activateTextureUnit(1);
    RenderSystem::Instance()->bindTexture(gFaceTex.get());
    RenderSystem::Instance()->setUniform1i(gShaderProgram, "texture2", 1);

    float timeValue = (float)Engine::Instance()->getExecuteTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    glm::vec4 color = glm::vec4(0.0f, greenValue, 0.0f, 1.0f);
    RenderSystem::Instance()->setUniform4f(gShaderProgram, "ourColor", color);

    glm::mat4 trans = glm::rotate(glm::mat4(1.f), glm::radians(10.f * (float)Engine::Instance()->getExecuteTime()), glm::vec3(0.0, 0.0, 1.0));
    RenderSystem::Instance()->setUniformMatrix4fv(gShaderProgram, "transform", trans);

    RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
        3, RenderProtocol::DrawElementType::UnsignedInt, 0);

    RenderSystem::Instance()->unbindShaderProgram();
    RenderSystem::Instance()->unbindVertexArray();
    RenderSystem::Instance()->unbindTexture(gWallTex.get());
    RenderSystem::Instance()->unbindTexture(gFaceTex.get());
}

void HelloTriangle01::initialize()       
{
    {
        gWallTex = TexMgr::Instance()->createTexture("wall.jpg", RenderProtocol::TexType::Texture2D);
        gFaceTex = TexMgr::Instance()->createTexture("awesomeface.png", RenderProtocol::TexType::Texture2D);
    }

    const std::string vsCode = Crash::FileSystem::ReadShader("HelloTriangle01_VS.txt");
    const std::string psCode = Crash::FileSystem::ReadShader("HelloTriangle01_PS.txt");
    gShaderProgram = RenderSystem::Instance()->createShaderProgram("HelloTriangle01", vsCode, psCode);


    gTriangleRP = BasicGeometry::CreateTriangleRP(BasicGeometry::ComFlag({
        BasicGeometry::DataType::Vertex, 
        BasicGeometry::DataType::TexCoord}));
}

void HelloTriangle01::shutdown()               
{
    gWallTex.reset();
    gFaceTex.reset();

    BasicGeometry::DestoryRenderPack(gTriangleRP);

    RenderSystem::Instance()->destroyShaderProgram(gShaderProgram);
    gShaderProgram = nullptr;
}
