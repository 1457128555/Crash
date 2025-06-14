 #include "CrashPCH.h"
 #include "HelloCube02.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashTexMgr.h"
#include "CrashFileSystem.h"
#include "CrashMaterial.h"
#include "CrashBasicGeometry.h"

using namespace Crash;

namespace
{
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
    
    BasicGeometry::RenderPack gCubeRP;

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
        RenderSystem::Instance()->bindVertexArray(gCubeRP.vao);
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

            RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
                36, RenderProtocol::DrawElementType::UnsignedInt, 0);
        }

        RenderSystem::Instance()->unbindShaderProgram();
        RenderSystem::Instance()->unbindVertexArray();
    }

    //  Render Dir Light
    {
        RenderSystem::Instance()->bindVertexArray(gCubeRP.vao);
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

        RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
            36, RenderProtocol::DrawElementType::UnsignedInt, 0);

        RenderSystem::Instance()->unbindShaderProgram();
        RenderSystem::Instance()->unbindVertexArray();
    }

    //  Render Cube
    {
        RenderSystem::Instance()->bindVertexArray(gCubeRP.vao);
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

            RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
                36, RenderProtocol::DrawElementType::UnsignedInt, 0);
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

    gCubeRP = BasicGeometry::CreateCubeRP(BasicGeometry::ComFlag({
        BasicGeometry::DataType::Vertex, 
        BasicGeometry::DataType::Normal,
        BasicGeometry::DataType::TexCoord}));

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

    BasicGeometry::DestoryRenderPack(gCubeRP);
   RenderSystem::Instance()->destroyShaderProgram(gShaderProgram);
   gShaderProgram = nullptr;
}
