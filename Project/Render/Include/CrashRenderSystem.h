
#pragma once

#include "CrashRenderDefines.hpp"
#include "CrashRenderProtocol.hpp"
#include "CrashSingleton.hpp"

#include <mutex>
namespace Crash
{
    class ShaderProgram;
    class VertexArrayObject;
    class VertexBuffer;
    class IndexBuffer;
    class Texture;

    class CRASH_RENDER_API RenderSystem : public Singleton<RenderSystem>
    {
    public:
        RenderSystem(void* procAddress, bool asyncRender);
        ~RenderSystem();

        void init();
        void uninit();
        
        void submitCommand();
        bool executeCommand();

        void frameBegin();
        void frameEnd();
        void setCullFaceEnable(bool enable);
        void setClearColor(const glm::vec4& color);
        void setViewport(const glm::ivec4& viewport);
        void clear(RenderProtocol::ClearFlag flag = RenderProtocol::ClearFlag::All);

        void setDepthEnable(bool enable);
        void setDepthFunc(RenderProtocol::CompareFunc func);
        void setClearDepth(float depth);

        void setStencilEnable(bool enable);
        void setStencilFunc(RenderProtocol::CompareFunc func, int ref, unsigned int mask);
        void setStencilOp(RenderProtocol::OperateFunc sfail, RenderProtocol::OperateFunc dpfail, RenderProtocol::OperateFunc dppass);
        void setStencilMask(unsigned int mask);

        ShaderProgram* createShaderProgram(const std::string& name, const std::string& vs, const std::string& fs);
        void destroyShaderProgram(ShaderProgram* program);
        void bindShaderProgram(ShaderProgram* program);
        void unbindShaderProgram();
        void setUniform1i(const ShaderProgram* program, const std::string& name, int value);
        void setUniform4f(const ShaderProgram* program, const std::string& name, const glm::vec4& value);
        void setUniformMatrix4fv(const ShaderProgram* program, const std::string& name, const glm::mat4& value);

        VertexArrayObject* createVertexArray();
        void destroyVertexArray(VertexArrayObject* vao);
        void bindVertexArray(VertexArrayObject* vao);
        void unbindVertexArray();
        void addBufferToVertexArray(VertexArrayObject* vao, VertexBuffer* buffer, unsigned int index, unsigned int size, unsigned int stride, const void* pointer);
        void addBufferToVertexArray(VertexArrayObject* vao, IndexBuffer* buffer);

        VertexBuffer* createBuffer();
        void destroyBuffer(VertexBuffer* buffer);
        void setBufferData(VertexBuffer* buffer, const void* data, unsigned int size);
       
        IndexBuffer* createIndexBuffer();
        void destroyIndexBuffer(IndexBuffer* buffer);
        void setIndexBufferData(IndexBuffer* buffer, const void* data, unsigned int size);

        Texture* createTexture(const std::string& name, RenderProtocol::TexType type);
        void destroyTexture(Texture* texture);

        void bindTexture(Texture* texture);
        void unbindTexture(Texture* texture);

       void setTextureData(Texture* texture, int level, RenderProtocol::TexFormat internalFormat, int width, int height, 
            RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data, bool generateMipmap);

        void setTextureWarpMode(Texture* texture, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround);
        void setTextureFilterMode(Texture* texture, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType);

        void activateTextureUnit(unsigned int unit);

        void drawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count);
        void drawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices);










        void setReverseZ(bool reverseZ) { mReverseZ = reverseZ; }
        bool getReverseZ() const { return mReverseZ; }

    private:
        void*   mProcAddress    = nullptr;
        bool    mAsyncRender    = false;
        bool    mInit           = false;
        bool    mReverseZ       = false;

        std::mutex mQueueMutex;
        std::vector<std::pair<unsigned int, void*>> mCommandQueue[3];
    };
}