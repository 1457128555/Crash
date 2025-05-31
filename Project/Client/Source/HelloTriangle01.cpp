#include "CrashPCH.h"
#include "HelloTriangle01.h"

#include "CrashRenderSystem.h"
#include "CrashFileSystem.h"
#include "CrashRenderBuffer.h"
#include "CrashTexture.h"
#include "CrashEngine.h"
#include "CrashTexMgr.h"

using namespace Crash;

namespace
{
    float vertices[] = {
        //  Position         // Texture Coords
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,     0.5f, 1.0f 
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    ShaderProgram*      gShaderProgram      = nullptr;
    VertexArrayObject*  gVertexArrayObject  = nullptr;
    VertexBuffer*       gVertexBuffer       = nullptr;
    IndexBuffer*        gIndexBuffer        = nullptr;
    Texture*            gWallTex            = nullptr;
    Texture*            gFaceTex            = nullptr;
}

HelloTriangle01::HelloTriangle01() : Scene("HelloTriangle01")
{
    
}

void HelloTriangle01::renderScene()
{
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    RenderSystem::Instance()->bindVertexArray(gVertexArrayObject);
    RenderSystem::Instance()->bindShaderProgram(gShaderProgram);

    RenderSystem::Instance()->activateTextureUnit(0);
    RenderSystem::Instance()->bindTexture(gWallTex);
    RenderSystem::Instance()->setUniform1i(gShaderProgram, "texture1", 0);

    RenderSystem::Instance()->activateTextureUnit(1);
    RenderSystem::Instance()->bindTexture(gFaceTex);
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
    RenderSystem::Instance()->unbindTexture(gWallTex);
    RenderSystem::Instance()->unbindTexture(gFaceTex);
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

    gVertexBuffer = RenderSystem::Instance()->createBuffer();
    RenderSystem::Instance()->setBufferData(gVertexBuffer, vertices, sizeof(vertices));

    gIndexBuffer = RenderSystem::Instance()->createIndexBuffer();
    RenderSystem::Instance()->setIndexBufferData(gIndexBuffer, indices, sizeof(indices));

    gVertexArrayObject = RenderSystem::Instance()->createVertexArray();
    RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gVertexBuffer, 0, 3, sizeof(float) * 5, (const void*)0);
    RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gVertexBuffer, 1, 2, sizeof(float) * 5, (const void*)(sizeof(float) * 3));
    RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gIndexBuffer);
    RenderSystem::Instance()->unbindVertexArray();
}

void HelloTriangle01::shutdown()               
{
    TexMgr::Instance()->destroyTexture(gWallTex);
    gWallTex = nullptr;
    TexMgr::Instance()->destroyTexture(gFaceTex);
    gFaceTex = nullptr;

    RenderSystem::Instance()->destroyBuffer(gVertexBuffer);
    gVertexBuffer = nullptr;
    RenderSystem::Instance()->destroyIndexBuffer(gIndexBuffer);
    gIndexBuffer = nullptr;
    RenderSystem::Instance()->destroyVertexArray(gVertexArrayObject);
    gVertexArrayObject = nullptr;

   RenderSystem::Instance()->destroyShaderProgram(gShaderProgram);
   gShaderProgram = nullptr;
}
