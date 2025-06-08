 #include "CrashPCH.h"
 #include "HelloBlending04.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashCamera.h"
#include "CrashFileSystem.h"
#include "CrashTexMgr.h"


using namespace Crash;

namespace
{
    ShaderProgram*      gPlaneShader    = nullptr;
    ShaderProgram*      gQuadShader    = nullptr;

    FrameBuffer*        gFrameBuffer    = nullptr;
    std::shared_ptr<Texture> gPlaneTex;
    std::shared_ptr<Texture> gVegTex;
    std::shared_ptr<Texture> gWinTex;

    Camera              gCamera;

    VertexArrayObject*  gPlaneVAO       = nullptr;
    VertexBuffer*       gPlaneVBO       = nullptr;

    VertexArrayObject*  gQuadVAO       = nullptr;
    VertexBuffer*       gQuadVBO       = nullptr;

    float planeVertices[] = {
        // positions            // texture Coords 
        -1.0f, 0.0f, -1.0f,    0.0f, 1.0f,
        -1.0f, 0.0f,  1.0f,    0.0f, 0.0f,
         1.0f, 0.0f,  1.0f,    1.0f, 0.0f,

         1.0f, 0.0f,  1.0f,    1.0f, 0.0f,
         1.0f, 0.0f, -1.0f,    1.0f, 1.0f,
        -1.0f, 0.0f, -1.0f,    0.0f, 1.0f,
    };

    float quadVertices[] = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };	

    std::vector<glm::vec3> vegetation = {
        {-1.5f,  0.0f, -0.48f},   
        { 1.5f,  0.0f,  0.51f},        
        { 0.0f,  0.0f,  0.7f },        
        {-0.3f,  0.0f, -2.3f },        
        { 0.5f,  0.0f, -0.6f },
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
    std::sort(window.begin(), window.end(), [](const glm::vec3& a, const glm::vec3& b) {
        glm::vec3 camPos = gCamera.getPosition();
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
        RenderSystem::Instance()->bindVertexArray(gPlaneVAO);
        RenderSystem::Instance()->bindShaderProgram(gPlaneShader);

        RenderSystem::Instance()->setUniform1i(gPlaneShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gPlaneTex.get());

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, 0.f)); 
        model = glm::scale(model, glm::vec3(50.0f)); 

        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uModel", model);
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uView", gCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uProjection", gCamera.getProjectionMat(Engine::Instance()->getAspect()));

        RenderSystem::Instance()->drawArray(RenderProtocol::DrawMode::Triangles, 0, 6);
    }

    //  Render Vegetation
    {
        RenderSystem::Instance()->bindVertexArray(gPlaneVAO);
        RenderSystem::Instance()->bindShaderProgram(gPlaneShader);

        RenderSystem::Instance()->setUniform1i(gPlaneShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gVegTex.get());

        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uView", gCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uProjection", gCamera.getProjectionMat(Engine::Instance()->getAspect()));
        
        for(auto &&veg : vegetation)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), veg);
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uModel", model);

            RenderSystem::Instance()->drawArray(RenderProtocol::DrawMode::Triangles, 0, 6);
        }
    }

    //  Render Window
    {
        RenderSystem::Instance()->bindVertexArray(gPlaneVAO);
        RenderSystem::Instance()->bindShaderProgram(gPlaneShader);

        RenderSystem::Instance()->setUniform1i(gPlaneShader, "uDiffuseTex", 0);
        RenderSystem::Instance()->activateTextureUnit(0);
        RenderSystem::Instance()->bindTexture(gWinTex.get());

        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uView", gCamera.getViewMat());
        RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uProjection", gCamera.getProjectionMat(Engine::Instance()->getAspect()));
       
        for(auto &&win : window)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), win);
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            RenderSystem::Instance()->setUniformMatrix4fv(gPlaneShader, "uModel", model);
            RenderSystem::Instance()->drawArray(RenderProtocol::DrawMode::Triangles, 0, 6);
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

        RenderSystem::Instance()->bindVertexArray(gQuadVAO);
        RenderSystem::Instance()->drawArray(RenderProtocol::DrawMode::Triangles, 0, 6);

        RenderSystem::Instance()->unbindVertexArray();
        RenderSystem::Instance()->unbindShaderProgram();
    }

}

void HelloBlending04::initialize()       
{

    Engine::Instance()->setControl(&gCamera);
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

    {
        gPlaneVAO = RenderSystem::Instance()->createVertexArray();
        gPlaneVBO = RenderSystem::Instance()->createBuffer();
        RenderSystem::Instance()->setBufferData(gPlaneVBO, planeVertices, sizeof(planeVertices));

        RenderSystem::Instance()->addBufferToVertexArray(gPlaneVAO, gPlaneVBO, 0, 3, sizeof(float) * 5, (const void*)0);
        RenderSystem::Instance()->addBufferToVertexArray(gPlaneVAO, gPlaneVBO, 1, 2, sizeof(float) * 5, (const void*)(sizeof(float) * 3));
        
        RenderSystem::Instance()->unbindVertexArray();
    }

    gFrameBuffer = RenderSystem::Instance()->createFrameBuffer("framebuffer", 
        Engine::Instance()->getWidth(), Engine::Instance()->getHeight(), true);

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloBlending04_Quad_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloBlending04_Quad_PS.txt");
        gQuadShader = RenderSystem::Instance()->createShaderProgram("HelloBlending04_Quad", vsCode, psCode);
  
    }

    {
        gQuadVAO = RenderSystem::Instance()->createVertexArray();
        gQuadVBO = RenderSystem::Instance()->createBuffer();
        RenderSystem::Instance()->setBufferData(gQuadVBO, quadVertices, sizeof(quadVertices));

        RenderSystem::Instance()->addBufferToVertexArray(gQuadVAO, gQuadVBO, 0, 2, sizeof(float) * 4, (const void*)0);
        RenderSystem::Instance()->addBufferToVertexArray(gQuadVAO, gQuadVBO, 1, 2, sizeof(float) * 4, (const void*)(sizeof(float) * 2));
        
        RenderSystem::Instance()->unbindVertexArray();
    }
}

void HelloBlending04::shutdown()               
{
    Engine::Instance()->setControl(nullptr);

    gPlaneTex.reset();
    gVegTex.reset();
    gWinTex.reset();

    RenderSystem::Instance()->destroyVertexArray(gPlaneVAO);
    RenderSystem::Instance()->destroyBuffer(gPlaneVBO);
    RenderSystem::Instance()->destroyShaderProgram(gPlaneShader);

    RenderSystem::Instance()->destroyFrameBuffer(gFrameBuffer);

    RenderSystem::Instance()->destroyVertexArray(gQuadVAO);
    RenderSystem::Instance()->destroyBuffer(gQuadVBO);
    RenderSystem::Instance()->destroyShaderProgram(gQuadShader);
}
