// #pragma once

// #include "CrashRenderDefines.hpp"
// #include "CrashSingleton.hpp"
// #include "CrashRenderProtocol.hpp"

// namespace Crash
// {
//   class CRASH_RENDER_API Render : public Singleton<Render>
//   {
//   public:
//     Render(void* procAddress);
//     ~Render();

//     void setPolygonMode(RenderProtocol::FaceMode face, RenderProtocol::PolygonMode polygon);

//     void setViewport(const std::array<int, 4>& viewport);

//     void setDepthEnable(bool enable);

//     void setClearDepth(float depth);

//     void clear(RenderProtocol::ClearFlag flag = RenderProtocol::ClearFlag::All);

//     unsigned int createShader(RenderProtocol::ShaderType type, const std::string& source);
//     void destroyShader(unsigned int id);

//     unsigned int createShaderProgram(const std::vector<unsigned int>& shaders);
//     void destroyShaderProgram(unsigned int id);
//     void bindShaderProgram(unsigned int id);
//     void unbindShaderProgram();

//     unsigned int createBuffer();
//     void destroyBuffer(unsigned int id);

//     void bindBuffer(unsigned int id, RenderProtocol::BufferType type);
//     void unbindBuffer(RenderProtocol::BufferType type);

//     int getUniformLocation(unsigned int shaderProgram, const std::string& name);
//     void setUniform1i(unsigned int locaID, int value);
//     void setUniform4f(unsigned int locaID, const float* value);

//     void bufferData(RenderProtocol::BufferType type, 
//           const void* data, unsigned int size, RenderProtocol::BufferUsage usage);

//     static void vertexAttribPointer(unsigned int index, unsigned int size, unsigned int stride, const void* pointer);  

//     unsigned int createVertexArray();
//     void destroyVertexArray(unsigned int id);
//     void bindVertexArray(unsigned int id);
//     void unbindVertexArray();
  
//     void drawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count);
//     void drawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices);

//     unsigned int createTexture();
//     void destroyTexture(unsigned int id);
//     void bindTexture(unsigned int id, RenderProtocol::TexType type);
//     void unbindTexture(RenderProtocol::TexType type);

//     static void setTextureData(RenderProtocol::TexType type, int level, RenderProtocol::TexFormat internalFormat,
//         int width, int height, RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data);
//     static void generateMipmap(RenderProtocol::TexType type);

//     void setTextureWarpMode(RenderProtocol::TexType type, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround);
//     void setTextureFilterMode(RenderProtocol::TexType type, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType);
//   private:
//     int getMaxVertexAttribs();
    
//   };
// }