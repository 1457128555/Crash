 #include "CrashPCH.h"
 #include "HelloCube02.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashTexMgr.h"
#include "CrashFileSystem.h"
#include "CrashMaterial.h"

using namespace Crash;

namespace
{
float cubeVertices[] = {
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
};

    unsigned int cubeIndices[] = {
        0,   1,  2,  2,  3,  0,       
        4,   5,  6,  6,  7,  4,       
        8,   9, 10, 10, 11,  8,       
        12, 13, 14, 14, 15, 12,       
        16, 17, 18, 18, 19, 16,       
        20, 21, 22, 22, 23, 20        
    };

    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    ShaderProgram*      gLightProgram       = nullptr;
    ShaderProgram*      gShaderProgram      = nullptr;
    VertexArrayObject*  gVertexArrayObject  = nullptr;
    VertexBuffer*       gVertexBuffer       = nullptr;
    IndexBuffer*        gIndexBuffer        = nullptr;

    std::shared_ptr<Texture> gContainerTex_D;
    std::shared_ptr<Texture> gContainerTex_S;

    Material            gMaterial;
    std::array<PointLight, 4> gPointLight;
}

HelloCube02::HelloCube02() : Scene("HelloCube02")
{
    
}

void HelloCube02::renderScene()
 {
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    //  Render Point Light
    {
        RenderSystem::Instance()->bindVertexArray(gVertexArrayObject);
        RenderSystem::Instance()->bindShaderProgram(gLightProgram);

        glm::mat4 view = mCamera.getViewMat();
        RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uView", view);
        
        glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
        RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uProjection", projection);
        
        RenderSystem::Instance()->setUniform4f(gLightProgram, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));
        
        for(auto &&pointLight : gPointLight)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(pointLight.getPosition()));
            model = glm::scale(model, glm::vec3(0.1f)); 
            RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uModel", model);

            pointLight.apply(gLightProgram);

            int indexCount = sizeof(cubeIndices) / sizeof(unsigned int);
            RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
                indexCount, RenderProtocol::DrawElementType::UnsignedInt, 0);
        }

        RenderSystem::Instance()->unbindShaderProgram();
        RenderSystem::Instance()->unbindVertexArray();
    }

    //  Render Dir Light
    {
        RenderSystem::Instance()->bindVertexArray(gVertexArrayObject);
        RenderSystem::Instance()->bindShaderProgram(gLightProgram);

        glm::mat4 model = glm::translate(glm::mat4(1.f), -50.f * glm::vec3(mDirLight.getDirection()));
        model = glm::scale(model, glm::vec3(4.f)); 
        RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uModel", model);

        glm::mat4 view = mCamera.getViewMat();
        RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uView", view);
    
        glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
        RenderSystem::Instance()->setUniformMatrix4fv(gLightProgram, "uProjection", projection);

        RenderSystem::Instance()->setUniform4f(gLightProgram, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));

        mDirLight.apply(gLightProgram);

        int indexCount = sizeof(cubeIndices) / sizeof(unsigned int);
        RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
            indexCount, RenderProtocol::DrawElementType::UnsignedInt, 0);

        RenderSystem::Instance()->unbindShaderProgram();
        RenderSystem::Instance()->unbindVertexArray();
    }

    //  Render Cube
    {
        RenderSystem::Instance()->bindVertexArray(gVertexArrayObject);
        RenderSystem::Instance()->bindShaderProgram(gShaderProgram);

        glm::mat4 view = mCamera.getViewMat();
        RenderSystem::Instance()->setUniformMatrix4fv(gShaderProgram, "uView", view);
    
        glm::mat4 projection = mCamera.getProjectionMat(Engine::Instance()->getAspect());
        RenderSystem::Instance()->setUniformMatrix4fv(gShaderProgram, "uProjection", projection);

        RenderSystem::Instance()->setUniform4f(gShaderProgram, "uCameraWorldPos", glm::vec4(mCamera.getPosition(), 1.0f));

        gMaterial.apply(gShaderProgram);
        for(int i = 0; i < 4; ++i)
            gPointLight[i].apply(gShaderProgram, i);
        
        mDirLight.apply(gShaderProgram);
        mSpotLight.apply(gShaderProgram);

        int cubeIndex = 0;
        for(const auto& p : cubePositions)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), p);
            model = glm::rotate(model, ++cubeIndex * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            RenderSystem::Instance()->setUniformMatrix4fv(gShaderProgram, "uModel", model);

            int indexCount = sizeof(cubeIndices) / sizeof(unsigned int);
            RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
                indexCount, RenderProtocol::DrawElementType::UnsignedInt, 0);
        }

        RenderSystem::Instance()->unbindShaderProgram();
        RenderSystem::Instance()->unbindVertexArray();
    }
}

void HelloCube02::initialize()       
{
    Scene::initialize();
    RenderSystem::Instance()->setCullFaceEnable(false);

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("PointLight_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("PointLight_PS.txt");
        gLightProgram = RenderSystem::Instance()->createShaderProgram("PointLight", vsCode, psCode);
    }

    {
        const std::string vsCode = Crash::FileSystem::ReadShader("HelloCube02_VS.txt");
        const std::string psCode = Crash::FileSystem::ReadShader("HelloCube02_PS.txt");
        gShaderProgram = RenderSystem::Instance()->createShaderProgram("HelloCube02", vsCode, psCode);
    }

    {
        gVertexBuffer = RenderSystem::Instance()->createBuffer();
        RenderSystem::Instance()->setBufferData(gVertexBuffer, cubeVertices, sizeof(cubeVertices));

        gIndexBuffer = RenderSystem::Instance()->createIndexBuffer();
        RenderSystem::Instance()->setIndexBufferData(gIndexBuffer, cubeIndices, sizeof(cubeIndices));

        gVertexArrayObject = RenderSystem::Instance()->createVertexArray();
        
        RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gVertexBuffer, 0, 3, sizeof(float) * 8, (const void*)0);
        RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gVertexBuffer, 1, 3, sizeof(float) * 8, (const void*)(sizeof(float) * 3));
        RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gVertexBuffer, 2, 2, sizeof(float) * 8, (const void*)(sizeof(float) * 6));
        RenderSystem::Instance()->addBufferToVertexArray(gVertexArrayObject, gIndexBuffer);
    
        RenderSystem::Instance()->unbindVertexArray();
    }

   //   Init Cube Material
    {
        gMaterial.setAmbient({1.0f, 0.5f, 0.31f});
        gMaterial.setDiffuse({1.0f, 0.5f, 0.31f});
        gMaterial.setSpecular({0.5f, 0.5f, 0.5f});
        gMaterial.setShininess(64.0f);

        gContainerTex_D = TexMgr::Instance()->createTexture("container2.png", RenderProtocol::TexType::Texture2D);
        gContainerTex_S = TexMgr::Instance()->createTexture("container2_specular.png", RenderProtocol::TexType::Texture2D);

        gMaterial.setDiffuseTex(gContainerTex_D);
        gMaterial.setSpecularTex(gContainerTex_S);
    }

    //  Init Point Light
    for(int i = 0; i < gPointLight.size(); ++i)
    {
        gPointLight[i].setPosition(pointLightPositions[i]);
        gPointLight[i].setAmbient({0.2f, 0.2f, 0.2f});
        gPointLight[i].setDiffuse({0.5f, 0.5f, 0.5f});
        gPointLight[i].setSpecular({1.0f, 1.0f, 1.0f});
    }
}

void HelloCube02::shutdown()               
{
    Scene::shutdown();

    gContainerTex_D.reset();
    gContainerTex_S.reset();

    gMaterial.reset();

    RenderSystem::Instance()->destroyBuffer(gVertexBuffer);
    gVertexBuffer = nullptr;
    RenderSystem::Instance()->destroyIndexBuffer(gIndexBuffer);
    gIndexBuffer = nullptr;
    RenderSystem::Instance()->destroyVertexArray(gVertexArrayObject);
    gVertexArrayObject = nullptr;

   RenderSystem::Instance()->destroyShaderProgram(gShaderProgram);
   gShaderProgram = nullptr;
}
