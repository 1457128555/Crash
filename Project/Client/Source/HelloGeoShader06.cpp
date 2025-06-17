 #include "CrashPCH.h"
 #include "HelloGeoShader06.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashCamera.h"
#include "CrashFileSystem.h"
#include "CrashTexMgr.h"
#include "CrashBasicGeometry.h"
#include "CrashRenderable.h"
#include "CrashRenderOperation.h"
#include "CrashRenderer.h"

using namespace Crash;

namespace
{
    std::vector<float> points = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  
    };

    class GeoRender : public Renderable
    {
    public:
        GeoRender()
        {
            {
                const std::string vsCode = Crash::FileSystem::ReadShader("HelloGeoRender06_VS.txt");
                const std::string psCode = Crash::FileSystem::ReadShader("HelloGeoRender06_PS.txt");
                const std::string gsCode = Crash::FileSystem::ReadShader("HelloGeoRender06_GS.txt");

                mShader = RenderSystem::Instance()->createShaderProgram("HelloGeoRender06", vsCode, psCode, gsCode);
            }

            {
                VertexBuffer* vbo = RenderSystem::Instance()->createBuffer();
                RenderSystem::Instance()->setBufferData(vbo, points.data(), sizeof(points[0]) * points.size());

                mRenderOpt.setVBO(vbo, {
                    { 0, 2, sizeof(float) * 5, (const void*)0 },                    
                    { 1, 3, sizeof(float) * 5, (const void*)(sizeof(float) * 2) },                    

                }, true);

                mRenderOpt.setCount(4);
                mRenderOpt.setDrawMode(RenderProtocol::DrawMode::Points);
                mRenderOpt.setRenderType(RenderOperation::RenderType::Array);
            }
        }
        
        ~GeoRender()
        {
            RenderSystem::Instance()->destroyShaderProgram(mShader);
        }

        virtual RenderOperation*    getRenderOperation()    {return &mRenderOpt;};
        virtual ShaderProgram*      getShaderProgram()      {return mShader;};
    private:
        ShaderProgram*      mShader    = nullptr;
        RenderOperation     mRenderOpt;
    };

    GeoRender*       gGeoRneder = nullptr;
}

void HelloGeoShader06::initialize()       
{
    Scene::initialize();
    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    gGeoRneder = new GeoRender();
}

void HelloGeoShader06::shutdown()               
{
    Scene::shutdown();

    delete gGeoRneder;
    gGeoRneder = nullptr;
}

void HelloGeoShader06::renderScene()
{
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    Renderer::Instance()->renderSingleObject(gGeoRneder);
}