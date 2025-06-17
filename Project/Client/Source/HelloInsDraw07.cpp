 #include "CrashPCH.h"
 #include "HelloInsDraw07.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashCamera.h"
#include "CrashFileSystem.h"
#include "CrashTexMgr.h"
#include "CrashBasicGeometry.h"
#include "CrashRenderable.h"
#include "CrashRenderOperation.h"
#include "CrashRenderer.h"
#include "CrashModel.h"

using namespace Crash;

namespace
{
    std::vector<float> quadVertices = {
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f,                   
    };  

    class _Render : public Renderable
    {
    public:
        _Render()
        {
            {
                int index = 0;
                float offset = 0.1f;
                for(int y = -10; y < 10; y += 2)
                {
                    for(int x = -10; x < 10; x += 2)
                    {
                        glm::vec2 translation;
                        translation.x = (float)x / 10.0f + offset;
                        translation.y = (float)y / 10.0f + offset;
                        mTrans[index++] = translation;
                    }
                }
            }

            {
                const std::string vsCode = Crash::FileSystem::ReadShader("HelloInsDraw07_VS.txt");
                const std::string psCode = Crash::FileSystem::ReadShader("HelloInsDraw07_PS.txt");

                mShader = RenderSystem::Instance()->createShaderProgram("HelloInsDraw07", vsCode, psCode);
            }

            {
                VertexBuffer* vbo = RenderSystem::Instance()->createBuffer();
                RenderSystem::Instance()->setBufferData(vbo, quadVertices.data(), sizeof(quadVertices[0]) * quadVertices.size());
                mRenderOpt.addVBO(vbo, {
                    { 0, 2, sizeof(float) * 5, (const void*)0 },                    
                    { 1, 3, sizeof(float) * 5, (const void*)(sizeof(float) * 2) },                    
                });

                VertexBuffer* insBuffer = RenderSystem::Instance()->createBuffer();
                RenderSystem::Instance()->setBufferData(insBuffer, mTrans.data(), sizeof(mTrans[0]) * mTrans.size());
                mRenderOpt.addVBO(insBuffer, {
                    { 2, 2, sizeof(float) * 2, (const void*)0 },                    
                }, 1);


                mRenderOpt.setCount(6);
                mRenderOpt.setInstanceCount(100);
                mRenderOpt.setRenderType(RenderOperation::RenderType::ArrayInstance);
                mRenderOpt.setCullFace(false);
            }
        }
        
        ~_Render()
        {
            RenderSystem::Instance()->destroyShaderProgram(mShader);
        }

        virtual void setUniformObject() override          
        {
            for(unsigned int i = 0; i < 100; i++)
                RenderSystem::Instance()->setUniform2f(mShader, "offsets[" + std::to_string(i) + "]", mTrans[i]);
        };

        virtual RenderOperation*    getRenderOperation()    {return &mRenderOpt;};
        virtual ShaderProgram*      getShaderProgram()      {return mShader;};
    private:
        ShaderProgram*      mShader    = nullptr;
        RenderOperation     mRenderOpt;

        std::array<glm::vec2, 100> mTrans;
    };

    _Render*        gRender = nullptr;
    // Model*          gPlanet = nullptr;
    // Model*          gRock   = nullptr;
}

void HelloInsDraw07::initialize()       
{
    Scene::initialize();
    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    // gPlanet = new Model("planet/planet.obj");
    // gRock = new Model("rock/rock.obj");

    //gRender = new _Render();
}

void HelloInsDraw07::shutdown()               
{
    Scene::shutdown();
    // delete gPlanet;
    // gPlanet = nullptr;
    // delete gRock;
    // gRock = nullptr;

    //delete gRender;
    //gRender = nullptr;
}

void HelloInsDraw07::renderScene()
{
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    //Renderer::Instance()->renderSingleObject(gRender);
}