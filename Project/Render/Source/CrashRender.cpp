// #include "CrashPCH.h"
// #include "CrashRender.h"

// #include "CrashRenderCommand.h"

// namespace Crash
// {
//     Render::Render(void* procAddress)
//     {
//         if(false == RenderCommand::Initlize(procAddress))
//         {
//             LogManager::Instance()->log("RenderCommand initialization failed");
//             assert(false && "RenderCommand initialization failed");
//         }
//     }

//     Render::~Render()
//     {
//         RenderCommand::Shutdown();
//     }

//     void Render::setPolygonMode(RenderProtocol::FaceMode face, RenderProtocol::PolygonMode polygon)
//     {
//         RenderCommand::SetPolygonMode(face, polygon);
//     }

//     void Render::setViewport(const std::array<int, 4>& viewport)
//     {
//         RenderCommand::SetViewport(viewport);
//     }

//     void Render::setDepthEnable(bool enable)
//     {
//         RenderCommand::SetDepthEnable(enable);
//     }

//     void Render::setClearDepth(float depth)
//     {
//         RenderCommand::SetClearDepth(depth);
//     }

//     void Render::clear(RenderProtocol::ClearFlag flag)
//     {
//         RenderCommand::Clear(flag);
//     }

//     unsigned int Render::createShader(RenderProtocol::ShaderType type, const std::string& source)
//     {
//         return RenderCommand::CreateShader(type, source);
//     }

//     void Render::destroyShader(unsigned int id)
//     {
//         RenderCommand::destroyShader(id);
//     }

//     unsigned int Render::createShaderProgram(const std::vector<unsigned int>& shaders)
//     {
//         return RenderCommand::CreateShaderProgram(shaders);
//     }

//     void Render::destroyShaderProgram(unsigned int id)
//     {
//         RenderCommand::destroyShaderProgram(id);
//     }

//     void Render::bindShaderProgram(unsigned int id)
//     {
//         RenderCommand::BindShaderProgram(id);
//     }

//     void Render::unbindShaderProgram()
//     {
//         RenderCommand::UnbindShaderProgram();
//     }

//     unsigned int Render::createBuffer()
//     {
//         return RenderCommand::CreateBuffer();
//     }   

//     void Render::destroyBuffer(unsigned int id)
//     {
//         RenderCommand::destroyBuffer(id);
//     }

//     void Render::bindBuffer(unsigned int id, RenderProtocol::BufferType type)
//     {
//         RenderCommand::BindBuffer(id, type);
//     }

//     void Render::unbindBuffer(RenderProtocol::BufferType type)
//     {
//         RenderCommand::UnbindBuffer(type);
//     }

//     int Render::getUniformLocation(unsigned int shaderProgram, const std::string& name)
//     {
//         return RenderCommand::GetUniformLocation(shaderProgram, name);
//     }

//     void Render::setUniform1i(unsigned int locaID, int value)
//     {
//         RenderCommand::SetUniform1i(locaID, value);
//     }
    
//     void Render::setUniform4f(unsigned int locaID, const float* value)
//     {
//         RenderCommand::SetUniform4f(locaID, value);
//     }

//     void Render::bufferData(RenderProtocol::BufferType type, const void* data, unsigned int size, RenderProtocol::BufferUsage usage)
//     {
//         RenderCommand::BufferData(type, data, size, usage);
//     }

//     void Render::vertexAttribPointer(unsigned int index, unsigned int size, unsigned int stride, const void* pointer)
//     {
//         RenderCommand::VertexAttribPointer(index, size, stride, pointer);
//     }

//     void Render::drawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count)
//     {
//         RenderCommand::DrawArray(mode, first, count);
//     }

//     void Render::drawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices)
//     {
//         RenderCommand::DrawElements(mode, count, type, indices);
//     }

//     unsigned int Render::createVertexArray()
//     {
//         return RenderCommand::CreateVertexArray();
//     }

//     void Render::destroyVertexArray(unsigned int id)
//     {
//         RenderCommand::destroyVertexArray(id);
//     }

//     void Render::bindVertexArray(unsigned int id)
//     {
//         RenderCommand::BindVertexArray(id);
//     }

//     void Render::unbindVertexArray()
//     {
//         RenderCommand::UnbindVertexArray();
//     }

//     int Render::getMaxVertexAttribs()
//     {
//         return RenderCommand::GetMaxVertexAttribs();
//     }

//     unsigned int Render::createTexture()
//     {
//         return RenderCommand::CreateTexture();
//     }

//     void Render::destroyTexture(unsigned int id)
//     {
//         RenderCommand::destroyTexture(id);
//     }

//     void Render::bindTexture(unsigned int id, RenderProtocol::TexType type)
//     {
//         RenderCommand::BindTexture(id, type);
//     }

//     void Render::unbindTexture(RenderProtocol::TexType type)
//     {
//         RenderCommand::UnbindTexture(type);
//     }

//     void Render::setTextureData(RenderProtocol::TexType type, int level, RenderProtocol::TexFormat internalFormat,
//         int width, int height, RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data)
//     {
//         RenderCommand::SetTextureData(type, level, internalFormat, width, height, format, dataType, data);
//     }

//     void Render::generateMipmap(RenderProtocol::TexType type)
//     {
//         RenderCommand::GenerateMipmap(type);
//     }

//     void Render::setTextureWarpMode(RenderProtocol::TexType type, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround)
//     {
//         RenderCommand::SetTextureWarpMode(type, wrap, surround);
//     }

//     void Render::setTextureFilterMode(RenderProtocol::TexType type, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType)
//     {
//         RenderCommand::SetTextureFilterMode(type, filter, filterType);
//     }
// } // namespace Crash