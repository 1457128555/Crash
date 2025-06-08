#pragma once

#include "CrashRenderProtocol.hpp"

namespace Crash
{
    class RenderCommand
    {
    public:
        static bool Initlize(void* procAddress);

        static void Shutdown();

        static void SetPolygonMode(RenderProtocol::FaceMode face, RenderProtocol::PolygonMode polygon);

        static void SetViewport(const glm::ivec4& viewport);

        static void SetClearColor(const glm::vec4& color);

        static void SetDepthEnable(bool enable);

        static void SetBlendEnable(bool enable);

        static void SetBlendFunc(RenderProtocol::BlendFunc src, RenderProtocol::BlendFunc dst);
        
        static void SetCullFaceEnable(bool enable);

        static void SetClearDepth(float depth);

        static void SetDetphFunc(RenderProtocol::CompareFunc func);

        static void SetStencilEnable(bool enable);

        static void SetStencilFunc(RenderProtocol::CompareFunc func, int ref, unsigned int mask);

        static void SetStencilOp(RenderProtocol::OperateFunc sfail, RenderProtocol::OperateFunc dpfail, RenderProtocol::OperateFunc dppass);

        static void SetStencilMask(unsigned int mask);

        static void Clear(RenderProtocol::ClearFlag flag = RenderProtocol::ClearFlag::All);

        static unsigned int CreateShader(RenderProtocol::ShaderType type, const std::string& source);
        static void destroyShader(unsigned int id);

        static unsigned int CreateShaderProgram(const std::vector<unsigned int>& shaders);
        static void destroyShaderProgram(unsigned int id);
        static void BindShaderProgram(unsigned int id);
        static void UnbindShaderProgram();

        static int GetUniformLocation(unsigned int shaderProgram, const std::string& name);
       
        static void SetUniform1i(unsigned int locaID, int value);
        static void SetUniform4f(unsigned int locaID, const glm::vec4& value);
        static void SetUniformMatrix4fv(unsigned int locaID, const glm::mat4& value);

        static unsigned int CreateRenderBufferObject();
        static void DestroyRenderBufferObject(unsigned int id);
        static void BindRenderBufferObject(unsigned int id);
        static void UnbindRenderBufferObject();

        static void SetRenderBufferStorage(RenderProtocol::TexFormat format, int width, int height);
        static void SerFrameBufferRenderBuffer(unsigned int id, RenderProtocol::AttachmentType type, unsigned int rboID);

        static unsigned int CreateFrameBuffer();
        static void destroyFrameBuffer(unsigned int id);

        static void BindFrameBuffer(unsigned int id);
        static void UnbindFrameBuffer();
        static bool CheckFrameBufferComplete();

        static void SetFramebufferTexture2D(unsigned int id,  RenderProtocol::AttachmentType type ,unsigned int textureID);

        static unsigned int CreateBuffer();
        static void destroyBuffer(unsigned int id);

        static void BindBuffer(unsigned int id, RenderProtocol::BufferType type);
        static void UnbindBuffer(RenderProtocol::BufferType type);

        static void BufferData(RenderProtocol::BufferType type, 
            const void* data, unsigned int size, RenderProtocol::BufferUsage usage);

        static void VertexAttribPointer(unsigned int index, unsigned int size, unsigned int stride, const void* pointer);  

        static unsigned int CreateVertexArray();
        static void destroyVertexArray(unsigned int id);
        static void BindVertexArray(unsigned int id);
        static void UnbindVertexArray();

        static void DrawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count);
        static void DrawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices);
    
        static unsigned int CreateTexture();
        static void destroyTexture(unsigned int id);
        static void BindTexture(unsigned int id, RenderProtocol::TexType type);
        static void UnbindTexture(RenderProtocol::TexType type);

        static void SetTextureData(RenderProtocol::TexType type, int level, RenderProtocol::TexFormat internalFormat,
            int width, int height, RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data);

        static void GenerateMipmap(RenderProtocol::TexType type);

        static void ActivateTextureUnit(unsigned int unit);

        static void SetTextureWarpMode(RenderProtocol::TexType type, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround);
        static void SetTextureFilterMode(RenderProtocol::TexType type, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType);
       
        static int GetMaxVertexAttribs();
    };  
} // namespace Crash