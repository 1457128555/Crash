 #include "CrashPCH.h"
 #include "HelloSkyCube05.h"

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
    class SkyCube : public Renderable
    {
    public:
        SkyCube()
        {
            {
                const std::string vsCode = Crash::FileSystem::ReadShader("HelloSkyCube05_VS.txt");
                const std::string psCode = Crash::FileSystem::ReadShader("HelloSkyCube05_PS.txt");
                mShader = RenderSystem::Instance()->createShaderProgram("HelloSkyCube05", vsCode, psCode);
            }

            {
                BasicGeometry::DataType dataType = BasicGeometry::ComFlag({
                BasicGeometry::DataType::Vertex, 
                BasicGeometry::DataType::Normal });
                
                std::vector<float> vertices;
                std::vector<unsigned int> indices;
                BasicGeometry::Cube(dataType,vertices, indices);

                IndexBuffer* ibo = RenderSystem::Instance()->createIndexBuffer();
                RenderSystem::Instance()->setIndexBufferData(ibo, indices.data(), sizeof(indices[0]) * indices.size());

                VertexBuffer* vbo = RenderSystem::Instance()->createBuffer();
                RenderSystem::Instance()->setBufferData(vbo, vertices.data(), sizeof(vertices[0]) * vertices.size());

                mRenderOpt.addIBO(ibo, true);
                mRenderOpt.addVBO(vbo, {
                    { 0, 3, sizeof(float) * 6, (const void*)0 },                    // Vertex
                    { 1, 3, sizeof(float) * 6, (const void*)(sizeof(float) * 3) }   // Normal
                });

                mRenderOpt.setCount(static_cast<unsigned int>(indices.size()));
            }
        }
        
        ~SkyCube()
        {
            RenderSystem::Instance()->destroyShaderProgram(mShader);
        }

        virtual RenderOperation*    getRenderOperation()    {return &mRenderOpt;};
        virtual ShaderProgram*      getShaderProgram()      {return mShader;};
    private:
        ShaderProgram*      mShader    = nullptr;
        RenderOperation     mRenderOpt;
    };

    class MirrorCube : public Renderable
    {
    public:
        MirrorCube()
        {
            {
                const std::string vsCode = Crash::FileSystem::ReadShader("HelloSkyCube05_Mirror_VS.txt");
                const std::string psCode = Crash::FileSystem::ReadShader("HelloSkyCube05_Mirror_PS.txt");
                mShader = RenderSystem::Instance()->createShaderProgram("HelloSkyCube05_Mirror", vsCode, psCode);
            }

           {
                BasicGeometry::DataType dataType = BasicGeometry::ComFlag({
                    BasicGeometry::DataType::Vertex, 
                    BasicGeometry::DataType::Normal });
                    
                std::vector<float> vertices;
                std::vector<unsigned int> indices;
                BasicGeometry::Cube(dataType,vertices, indices);

                IndexBuffer* ibo = RenderSystem::Instance()->createIndexBuffer();
                RenderSystem::Instance()->setIndexBufferData(ibo, indices.data(), sizeof(indices[0]) * indices.size());

                VertexBuffer* vbo = RenderSystem::Instance()->createBuffer();
                RenderSystem::Instance()->setBufferData(vbo, vertices.data(), sizeof(vertices[0]) * vertices.size());

                mRenderOpt.addIBO(ibo, true);
                mRenderOpt.addVBO(vbo, {
                    { 0, 3, sizeof(float) * 6, (const void*)0 },                    // Vertex
                    { 1, 3, sizeof(float) * 6, (const void*)(sizeof(float) * 3) }   // Normal
                });

                mRenderOpt.setCount(static_cast<unsigned int>(indices.size()));
           }
        }

        ~MirrorCube()
        {
            RenderSystem::Instance()->destroyShaderProgram(mShader);
        }

        virtual RenderOperation*    getRenderOperation()    {return &mRenderOpt;};
        virtual ShaderProgram*      getShaderProgram()      {return mShader;};

    private:
        ShaderProgram*      mShader    = nullptr;
        RenderOperation     mRenderOpt;
    };

    SkyCube*        gSkyCube = nullptr;
    MirrorCube*     gMirrorCube = nullptr;
}

void HelloSkyCube05::initialize()       
{
    Scene::initialize();
    RenderSystem::Instance()->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

    gSkyCube = new SkyCube();
    gMirrorCube = new MirrorCube();
}

void HelloSkyCube05::shutdown()               
{
    Scene::shutdown();

    delete gSkyCube;
    gSkyCube = nullptr;

    delete gMirrorCube;
    gMirrorCube = nullptr;
}

void HelloSkyCube05::renderScene()
{
    RenderSystem::Instance()->clear(RenderProtocol::ClearFlag::All);

    RenderSystem::Instance()->setCullFaceEnable(false);
    RenderSystem::Instance()->setDepthFunc(RenderProtocol::CompareFunc::LessEqual);

    Renderer::Instance()->renderSingleObject(gMirrorCube);
    Renderer::Instance()->renderSingleObject(gSkyCube);
}