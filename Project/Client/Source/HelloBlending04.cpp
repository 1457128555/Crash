#include "CrashPCH.h"
#include "HelloBlending04.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashFileSystem.h"
#include "CrashTexMgr.h"
#include "CrashBasicGeometry.h"

using namespace Crash;

namespace
{
    ShaderProgram *gPlaneShader = nullptr;
    ShaderProgram *gQuadShader = nullptr;

    FrameBuffer *gFrameBuffer = nullptr;
    std::shared_ptr<Texture> gPlaneTex;
    std::shared_ptr<Texture> gVegTex;
    std::shared_ptr<Texture> gWinTex;

    BasicGeometry::RenderPack gQuadRP;

    std::vector<glm::vec3> vegetation = {
        {-1.5f, 0.0f, -0.48f},
        {1.5f, 0.0f, 0.51f},
        {0.0f, 0.0f, 0.7f},
        {-0.3f, 0.0f, -2.3f},
        {0.5f, 0.0f, -0.6f},
    };

    std::vector<glm::vec3> window = {
        {0.f, 0.f, 4.f},
        {0.f, 0.f, 1.f},
        {0.f, 0.f, 2.f},
    };
}

HelloBlending04::HelloBlending04() : Scene("HelloBlending04")
{
}

void HelloBlending04::update(float deltaTime)
{
    Scene::update(deltaTime);

    std::sort(window.begin(), window.end(), [&](const glm::vec3 &a, const glm::vec3 &b)
              {
                  glm::vec3 camPos = mCamera.getPosition();
                  return glm::length(camPos - a) > glm::length(camPos - b); // Sort by z-coordinate
              });
}

void HelloBlending04::renderScene()
{
    RenderSystem::Instance()->bindFrameBuffer(gFrameBuffer);
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);
    RenderSystem::Instance()->setDepthEnable(true);

    //  Render Plane
    {
        RenderSystem::Instance()->bindVertexArray(gQuadRP.vao);
        RenderSystem::Instance()->bindShaderProgram(gPlaneShader);

        RenderSystem::Instance()->setUniform1i(gPlaneShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gPlaneTex.get());

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, 0.f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(50.0f));

        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uModel", model);
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uView", mCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uProjection", mCamera.getProjectionMat(Engine::Instance()->getAspect()));

        RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles,
                                               6, RenderProtocol::DrawElementType::UnsignedInt, 0);
    }

    //  Render Vegetation
    {
        RenderSystem::Instance()->bindVertexArray(gQuadRP.vao);
        RenderSystem::Instance()->bindShaderProgram(gPlaneShader);

        RenderSystem::Instance()->setUniform1i(gPlaneShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gVegTex.get());

        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uView", mCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uProjection", mCamera.getProjectionMat(Engine::Instance()->getAspect()));

        for (auto &&veg : vegetation)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), veg);
            RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uModel", model);

            RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
                6, RenderProtocol::DrawElementType::UnsignedInt, 0);
        }
    }

    //  Render Window
    {
        RenderSystem::Instance()->bindVertexArray(gQuadRP.vao);
        RenderSystem::Instance()->bindShaderProgram(gPlaneShader);

        RenderSystem::Instance()->setUniform1i(gPlaneShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gWinTex.get());

        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uView", mCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uProjection", mCamera.getProjectionMat(Engine::Instance()->getAspect()));

        for (auto &&win : window)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), win);
            RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uModel", model);
            RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
                6, RenderProtocol::DrawElementType::UnsignedInt, 0);
        }
    }

    RenderSystem::Instance()->unbindFrameBuffer();
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    RenderSystem::Instance()->setDepthEnable(false);

    //  Render FrameBuffer to Screen
    {
        RenderSystem::Instance()->bindShaderProgram(gQuadShader);
        RenderSystem::Instance()->setUniform1i(gQuadShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(RenderSystem::Instance()->getFrameBufferColorAttachment(gFrameBuffer).get());

        RenderSystem::Instance()->bindVertexArray(gQuadRP.vao);
        RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
            6, RenderProtocol::DrawElementType::UnsignedInt, 0);

        RenderSystem::Instance()->unbindVertexArray();
        RenderSystem::Instance()->unbindShaderProgram();
    }
}

void HelloBlending04::initialize()
{
    Scene::initialize();

    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    gPlaneTex = TexMgr::Instance()->createTexture("marble.jpg", RenderProtocol::TexType::Texture2D);
    gVegTex = TexMgr::Instance()->createTexture("grass.png", RenderProtocol::TexType::Texture2D);
    RenderSystem::Instance()->setTextureWarpMode(gVegTex.get(), RenderProtocol::TexWrap::S, RenderProtocol::TexSurround::ClampToEdge);
    RenderSystem::Instance()->setTextureWarpMode(gVegTex.get(), RenderProtocol::TexWrap::T, RenderProtocol::TexSurround::ClampToEdge);

    gWinTex = TexMgr::Instance()->createTexture("blending_transparent_window.png", RenderProtocol::TexType::Texture2D);
    RenderSystem::Instance()->setTextureWarpMode(gWinTex.get(), RenderProtocol::TexWrap::S, RenderProtocol::TexSurround::ClampToEdge);
    RenderSystem::Instance()->setTextureWarpMode(gWinTex.get(), RenderProtocol::TexWrap::T, RenderProtocol::TexSurround::ClampToEdge);

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloBlending04_Plane_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloBlending04_Plane_PS.txt");
        gPlaneShader = RenderSystem::Instance()->createShaderProgram("HelloBlending04_Plane", vsCode, psCode);
    }

    gFrameBuffer = RenderSystem::Instance()->createFrameBuffer("framebuffer",
                                                               Engine::Instance()->getWidth(), Engine::Instance()->getHeight());

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloBlending04_Quad_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloBlending04_Quad_PS.txt");
        gQuadShader = RenderSystem::Instance()->createShaderProgram("HelloBlending04_Quad", vsCode, psCode);
    }

    gQuadRP = BasicGeometry::CreateQuadRP(BasicGeometry::ComFlag({BasicGeometry::DataType::Vertex,
                                                                BasicGeometry::DataType::TexCoord}));
}

void HelloBlending04::shutdown()
{
    Scene::shutdown();

    gPlaneTex.reset();
    gVegTex.reset();
    gWinTex.reset();

    BasicGeometry::DestoryRenderPack(gQuadRP);

    RenderSystem::Instance()->destroyShaderProgram(gPlaneShader);
    RenderSystem::Instance()->destroyShaderProgram(gQuadShader);

    RenderSystem::Instance()->destroyFrameBuffer(gFrameBuffer);
}
