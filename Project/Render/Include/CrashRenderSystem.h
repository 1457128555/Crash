
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
    class UniformBuffer;
    class IndexBuffer;
    class Texture;
    class FrameBuffer;
    class RenderBuffer;

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

        ShaderProgram* createShaderProgram(const std::string& name, 
            const std::string& vs, const std::string& fs, const std::string& gs = "");
        
        void destroyShaderProgram(ShaderProgram* program);
        void bindShaderProgram(ShaderProgram* program);
        void unbindShaderProgram();
        void setUniform1i(const ShaderProgram* program, const std::string& name, int value);
        void setUniform2f(const ShaderProgram* program, const std::string& name, const glm::vec2& value);
        void setUniform4f(const ShaderProgram* program, const std::string& name, const glm::vec4& value);
        void setUniformMatrix4fv(const ShaderProgram* program, const std::string& name, const glm::mat4& value);
        void setUniformBlockBinding(const ShaderProgram* program, const std::string& blockName, unsigned int bindingPoint);

        FrameBuffer* createFrameBuffer(const std::string& name, unsigned int width, unsigned int height, int multiSamples, bool useRBO);
        void destroyFrameBuffer(FrameBuffer* framebuffer);
        void bindFrameBuffer(FrameBuffer* framebuffer);
        void unbindFrameBuffer();
        void blitFrameBuffer(FrameBuffer* r, FrameBuffer* w);

        const std::shared_ptr<Texture>& getFrameBufferColorAttachment(FrameBuffer* framebuffer)         const;
        const std::shared_ptr<Texture>& getFrameBufferDepthStencilAttachment(FrameBuffer* framebuffer)  const;

        VertexArrayObject* createVertexArray();
        void destroyVertexArray(VertexArrayObject* vao);
        void bindVertexArray(VertexArrayObject* vao);
        void unbindVertexArray();
        void addBufferToVertexArray(VertexArrayObject* vao, VertexBuffer* buffer, unsigned int index, 
            unsigned int size, unsigned int stride, const void* pointer, unsigned int insStep);
        void addBufferToVertexArray(VertexArrayObject* vao, IndexBuffer* buffer);

        VertexBuffer* createBuffer();
        void destroyBuffer(VertexBuffer* buffer);
        
        void setBufferData(RenderBuffer* buffer, const void* data, unsigned int size);
        void setBufferSubData(RenderBuffer* buffer, unsigned int offset, const void* data, unsigned int size);

        UniformBuffer* createUniformBuffer();
        void destroyUniformBuffer(UniformBuffer* buffer);
        void setBindBufferBase(UniformBuffer* buffer, unsigned int bindingPoint);

        IndexBuffer* createIndexBuffer();
        void destroyIndexBuffer(IndexBuffer* buffer);
        void setIndexBufferData(IndexBuffer* buffer, const void* data, unsigned int size);

        Texture* createTexture(const std::string& name, RenderProtocol::TexType type);
        void destroyTexture(Texture* texture);

        void bindTexture(Texture* texture);
        void unbindTexture(Texture* texture);

        void setCubeMapTextureData(Texture* texture, RenderProtocol::TexFormat internalFormat, int width, int height, 
            RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, std::array<const void*, 6> data, bool generateMipmap);

       void setTextureData(Texture* texture, int level, RenderProtocol::TexFormat internalFormat, int width, int height, 
            RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data, bool generateMipmap);

        void setTextureWarpMode(Texture* texture, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround);
        void setTextureFilterMode(Texture* texture, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType);

        void activateTextureUnit(unsigned int unit);

        void drawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count);
        void drawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices);

        void drawArraysInstanced(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count, unsigned int instanceCount);
        void drawElementsInstanced(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices, unsigned int instanceCount);









        void setReverseZ(bool reverseZ) { mReverseZ = reverseZ; }
        bool getReverseZ() const { return mReverseZ; }

    private:
        void*   mProcAddress    = nullptr;
        bool    mAsyncRender    = false;
        bool    mInit           = false;
        bool    mReverseZ       = false;

        std::mutex mQueueMutex;
        std::vector<std::function<void()>> mCommandQueue[3];
    };
}