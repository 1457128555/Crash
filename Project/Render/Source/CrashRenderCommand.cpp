#include "CrashPCH.h"
#include "CrashRenderCommand.h"

#include "CrashRenderSystem.h"
#include <glad/glad.h>

namespace Crash
{
    namespace
    {

        void CheckGLError(const std::string& msg) 
        {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) 
            {
                assert(err != GL_NO_ERROR && "OpenGL error occurred!");
                LogManager::Instance()->log("OpenGL Error: " + std::to_string(err) + " - " + msg);
            }
        }

        GLenum GetCompareFunc(RenderProtocol::CompareFunc func)
        {
            switch (func)
            {
                case RenderProtocol::CompareFunc::Never:         return GL_NEVER;
                case RenderProtocol::CompareFunc::Always:        return GL_ALWAYS;
                case RenderProtocol::CompareFunc::Less:          return GL_LESS;
                case RenderProtocol::CompareFunc::LessEqual:     return GL_LEQUAL;
                case RenderProtocol::CompareFunc::Equal:         return GL_EQUAL;
                case RenderProtocol::CompareFunc::GreaterEqual:  return GL_GEQUAL;
                case RenderProtocol::CompareFunc::Greater:       return GL_GREATER;
                case RenderProtocol::CompareFunc::NotEqual:      return GL_NOTEQUAL;
                default: assert(false && "Invalid compare function"); return 0;
            }
        }

        GLenum GetOperateFunc(RenderProtocol::OperateFunc func)
        {
            switch (func)
            {
                case RenderProtocol::OperateFunc::Keep:           return GL_KEEP;
                case RenderProtocol::OperateFunc::Zero:           return GL_ZERO;
                case RenderProtocol::OperateFunc::Replace:        return GL_REPLACE;
                case RenderProtocol::OperateFunc::Invert:         return GL_INVERT;
                case RenderProtocol::OperateFunc::Increment:      return GL_INCR;
                case RenderProtocol::OperateFunc::Decrement:      return GL_DECR;
                case RenderProtocol::OperateFunc::IncrementWrap:  return GL_INCR_WRAP;
                case RenderProtocol::OperateFunc::DecrementWrap:  return GL_DECR_WRAP;
                default: assert(false && "Invalid operate function"); return 0;
            }
        }

        GLenum GetTexFormat(RenderProtocol::TexFormat format)
        {
            switch (format)
            {
                case RenderProtocol::TexFormat::Red:   return GL_RED;
                case RenderProtocol::TexFormat::Green: return GL_GREEN;
                case RenderProtocol::TexFormat::Blue:  return GL_BLUE;
                case RenderProtocol::TexFormat::Alpha: return GL_ALPHA;
                case RenderProtocol::TexFormat::RGB:   return GL_RGB;
                case RenderProtocol::TexFormat::RGBA:  return GL_RGBA;
                default: assert(false && "Invalid texture format"); return 0;
            }
            return 0;
        }

        GLenum GetTexDataType(RenderProtocol::TexDataType dataType)
        {
            switch (dataType)
            {
                case RenderProtocol::TexDataType::UnsignedByte:   return GL_UNSIGNED_BYTE;
                case RenderProtocol::TexDataType::UnsignedShort:  return GL_UNSIGNED_SHORT;
                case RenderProtocol::TexDataType::UnsignedInt:    return GL_UNSIGNED_INT;
                case RenderProtocol::TexDataType::Float:          return GL_FLOAT;
                default: assert(false && "Invalid texture data type"); return 0;
            }
            return 0;
        }
  
        GLenum GetTexType(RenderProtocol::TexType type)
        {
            switch (type)
            {
                case RenderProtocol::TexType::Texture1D:      return GL_TEXTURE_1D;
                case RenderProtocol::TexType::Texture2D:      return GL_TEXTURE_2D;
                case RenderProtocol::TexType::Texture3D:      return GL_TEXTURE_3D;
                case RenderProtocol::TexType::TextureCubeMap: return GL_TEXTURE_CUBE_MAP;
                default: assert(false && "Invalid texture type"); return 0;
            }
            return 0;
        }

        GLenum GetTexSurround(RenderProtocol::TexSurround surround)
        {
            switch (surround)
            {
                case RenderProtocol::TexSurround::Repeat:         return GL_REPEAT;
                case RenderProtocol::TexSurround::MirroredRepeat: return GL_MIRRORED_REPEAT;
                case RenderProtocol::TexSurround::ClampToBorder:  return GL_CLAMP_TO_BORDER;
                case RenderProtocol::TexSurround::ClampToEdge:    return GL_CLAMP_TO_EDGE;
                default: assert(false && "Invalid texture surround mode"); return 0;
            }
            return 0;
        }

        GLenum GetTexWrap(RenderProtocol::TexWrap wrap)
        {
            switch (wrap)
            {
                case RenderProtocol::TexWrap::S:   return GL_TEXTURE_WRAP_S;
                case RenderProtocol::TexWrap::T:   return GL_TEXTURE_WRAP_T;
                case RenderProtocol::TexWrap::R:   return GL_TEXTURE_WRAP_R;
                default: assert(false && "Invalid texture wrap mode"); return 0;
            }
            return 0;
        }

        GLenum GetTexFilter(RenderProtocol::TexFilter filter)
        {
            switch (filter)
            {
                case RenderProtocol::TexFilter::MinFilter :                 return GL_TEXTURE_MIN_FILTER;
                case RenderProtocol::TexFilter::MagFilter :                 return GL_TEXTURE_MAG_FILTER;
                default: assert(false && "Invalid texture filter mode");    return 0;
            }
            return 0;
        }

        GLenum GetTexFilterType(RenderProtocol::TexFilterType filterType)
        {
            switch (filterType)
            {
                case RenderProtocol::TexFilterType::Nearest:                return GL_NEAREST;
                case RenderProtocol::TexFilterType::Linear:                 return GL_LINEAR;
                case RenderProtocol::TexFilterType::NearestMipmapNearest:   return GL_NEAREST_MIPMAP_NEAREST;
                case RenderProtocol::TexFilterType::LinearMipmapNearest:    return GL_LINEAR_MIPMAP_NEAREST;
                case RenderProtocol::TexFilterType::NearestMipmapLinear:    return GL_NEAREST_MIPMAP_LINEAR;
                case RenderProtocol::TexFilterType::LinearMipmapLinear:     return GL_LINEAR_MIPMAP_LINEAR;
                default: assert(false && "Invalid texture filter type");    return 0;
            }
            return 0;
        }

        GLenum GetBufferType(RenderProtocol::BufferType type)
        {
            switch (type)
            {
                case RenderProtocol::BufferType::VertexBuffer: return GL_ARRAY_BUFFER;
                case RenderProtocol::BufferType::IndexBuffer:  return GL_ELEMENT_ARRAY_BUFFER;
                default: assert(false && "Invalid buffer type"); return 0;
            }
            return 0;
        }

        GLenum GetBufferUsage(RenderProtocol::BufferUsage usage)
        {
            switch (usage)
            {
                case RenderProtocol::BufferUsage::StaticDraw:  return GL_STATIC_DRAW;
                case RenderProtocol::BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
                case RenderProtocol::BufferUsage::StreamDraw:  return GL_STREAM_DRAW;
                default: assert(false && "Invalid buffer usage"); return 0;
            }
            return 0;
        }

        GLenum GetDrawMode(RenderProtocol::DrawMode mode)
        {
            switch (mode)
            {
                case RenderProtocol::DrawMode::Points:         return GL_POINTS;
                case RenderProtocol::DrawMode::Lines:          return GL_LINES;
                case RenderProtocol::DrawMode::LineStrip:      return GL_LINE_STRIP;
                case RenderProtocol::DrawMode::Triangles:      return GL_TRIANGLES;
                case RenderProtocol::DrawMode::TriangleStrip:  return GL_TRIANGLE_STRIP;
                case RenderProtocol::DrawMode::TriangleFan:    return GL_TRIANGLE_FAN;
                default: assert(false && "Invalid draw mode"); return 0;
            }
            return 0;
        }

        GLenum GetDrawElementType(RenderProtocol::DrawElementType type)
        {
            switch (type)
            {
                case RenderProtocol::DrawElementType::UnsignedByte:   return GL_UNSIGNED_BYTE;
                case RenderProtocol::DrawElementType::UnsignedShort:  return GL_UNSIGNED_SHORT;
                case RenderProtocol::DrawElementType::UnsignedInt:    return GL_UNSIGNED_INT;
                default: assert(false && "Invalid draw element type"); return 0;
            }
            return 0;
        }

        GLenum GetFaceMode(RenderProtocol::FaceMode face)
        {
            switch (face)
            {
                case RenderProtocol::FaceMode::Front:  return GL_FRONT;
                case RenderProtocol::FaceMode::Back:   return GL_BACK;
                case RenderProtocol::FaceMode::FrontAndBack: return GL_FRONT_AND_BACK;
                default: assert(false && "Invalid face mode"); return 0;
            }
            return 0;
        }

        GLenum GetPolygonMode(RenderProtocol::PolygonMode polygon)
        {
            switch (polygon)
            {
                case RenderProtocol::PolygonMode::Fill:  return GL_FILL;
                case RenderProtocol::PolygonMode::Line:  return GL_LINE;
                case RenderProtocol::PolygonMode::Point: return GL_POINT;
                default: assert(false && "Invalid polygon mode"); return 0;
            }
            return 0;
        }

        GLenum GetShaderType(RenderProtocol::ShaderType type)
        {
            switch (type)
            {
                case RenderProtocol::ShaderType::VertexShader:   return GL_VERTEX_SHADER;
                case RenderProtocol::ShaderType::FragmentShader: return GL_FRAGMENT_SHADER;
                case RenderProtocol::ShaderType::GeometryShader: return GL_GEOMETRY_SHADER;
                case RenderProtocol::ShaderType::ComputeShader:  return GL_COMPUTE_SHADER;
                default: assert(false && "Invalid shader type"); return 0;
            }
            return 0;
        }
    }

    bool RenderCommand::Initlize(void* procAddress)
    {
        LogManager::Instance()->log("RenderCommand initialized");
    #ifdef _WIN32
        if (!gladLoadGLLoader((GLADloadproc)procAddress))
    #else
        if (!gladLoadGLES2Loader((GLADloadproc)procAddress)) 
    #endif
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        CheckGLError("GLAD initialization");

        const char* glVersion   = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        const char* glVendor    = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* glRenderer  = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

        LogManager::Instance()->log(std::string("OpenGL Version: ")     + (glVersion    ? glVersion     : "Unknown"));
        LogManager::Instance()->log(std::string("GLSL Version: ")       + (glslVersion  ? glslVersion   : "Unknown"));
        LogManager::Instance()->log(std::string("Vendor: ")             + (glVendor     ? glVendor      : "Unknown"));
        LogManager::Instance()->log(std::string("Renderer: ")           + (glRenderer   ? glRenderer    : "Unknown"));

        //SetViewport({0, 0, 1280, 720});
        SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
        SetCullFaceEnable(true);
 
        SetDepthEnable(true);
        if(RenderSystem::Instance()->getReverseZ())
        {
            SetDetphFunc(RenderProtocol::CompareFunc::Greater);
            SetClearDepth(0.f);
        }
        else
        {

            SetDetphFunc(RenderProtocol::CompareFunc::Less);
            SetClearDepth(1.f);
        }
        
        return true;
    }

    void RenderCommand::Shutdown()
    {
        LogManager::Instance()->log("RenderCommand shutdown");
    }

    void RenderCommand::SetPolygonMode(RenderProtocol::FaceMode face, RenderProtocol::PolygonMode polygon)
    {
        glPolygonMode(GetFaceMode(face), GetPolygonMode(polygon));
        CheckGLError("SetPolygonMode");
    }

    void RenderCommand::SetViewport(const glm::ivec4& viewport)
    {
        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
        CheckGLError("SetViewport");
    }

    void RenderCommand::Clear(RenderProtocol::ClearFlag flag)
    {
        GLbitfield mask = 0;
        if (static_cast<unsigned int>(flag) & static_cast<unsigned int>(RenderProtocol::ClearFlag::Color))
            mask |= GL_COLOR_BUFFER_BIT;
        if (static_cast<unsigned int>(flag) & static_cast<unsigned int>(RenderProtocol::ClearFlag::Depth))
            mask |= GL_DEPTH_BUFFER_BIT;
        if (static_cast<unsigned int>(flag) & static_cast<unsigned int>(RenderProtocol::ClearFlag::Stencil))
            mask |= GL_STENCIL_BUFFER_BIT;

        glClear(mask);
        CheckGLError("Clear");
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        CheckGLError("SetClearColor");
    }

    void RenderCommand::SetDepthEnable(bool enable)
    {
        if(enable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);    
        CheckGLError("SetDepthEnable");
    }

    void RenderCommand::SetCullFaceEnable(bool enable)
    {
        if(enable)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);    
        CheckGLError("SetCullFaceEnable");
    }

    void RenderCommand::SetClearDepth(float depth)
    {
#ifdef _WIN32
        glClearDepth(depth);
#else
        glClearDepthf(depth);
#endif
        CheckGLError("SetClearDepth");
    }

    void RenderCommand::SetDetphFunc(RenderProtocol::CompareFunc func)
    {
        glDepthFunc(GetCompareFunc(func));
        CheckGLError("SetDetphFunc");
    }

    unsigned int RenderCommand::CreateShader(RenderProtocol::ShaderType type, const std::string& source)
    {
        unsigned int id = glCreateShader(GetShaderType(type));
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);
            LogManager::Instance()->log("ERROR::SHADER_COMPILATION_ERROR\n" + std::string(infoLog));
            LogManager::Instance()->log(source);

            assert(false && "Shader compilation failed!");
        }

        CheckGLError("CreateShader");
        return id;
    }

    void RenderCommand::destroyShader(unsigned int id)
    {
        assert(id && "shader is null!");
        glDeleteShader(id);
        CheckGLError("destroyShader");
    }

    unsigned int RenderCommand::CreateShaderProgram(const std::vector<unsigned int>& shaders)
    {
        unsigned int id = glCreateProgram();
        for (const auto& shader : shaders)
            glAttachShader(id, shader);
        
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
            LogManager::Instance()->log("ERROR::PROGRAM_LINKING_ERROR\n" + std::string(infoLog));
        }

        for (const auto& shader : shaders)
            glDetachShader(id, shader);


        CheckGLError("CreateShaderProgram");    
        return id;
    }

    void RenderCommand::destroyShaderProgram(unsigned int id)
    {
        assert(id && "Shader Program is null!");
        glDeleteProgram(id);
        CheckGLError("destroyShaderProgram");
    }

    void RenderCommand::BindShaderProgram(unsigned int id)
    {
        assert(id && "Shader Program is null!");
        glUseProgram(id);
        CheckGLError("BindShaderProgram");
    }

    void RenderCommand::UnbindShaderProgram()
    {
        glUseProgram(0);
        CheckGLError("UnbindShaderProgram");
    }

    int RenderCommand::GetUniformLocation(unsigned int shaderProgram, const std::string& name)
    {
        assert(shaderProgram && "Shader Program is null!");
        int ret = glGetUniformLocation(shaderProgram, name.c_str());
        CheckGLError("GetUniformLocation");
        return ret;
    }

    void RenderCommand::SetUniform1i(unsigned int locaID, int value)
    {
        glUniform1i(locaID, value);
        CheckGLError("SetUniform1i");
    }

    void RenderCommand::SetUniform4f(unsigned int locaID, const glm::vec4& value)
    {
        glUniform4f(locaID, value.x, value.y, value.z, value.w);
        CheckGLError("SetUniform4f");
    }

    void RenderCommand::SetUniformMatrix4fv(unsigned int locaID, const glm::mat4& value)
    {
        glUniformMatrix4fv(locaID, 1, GL_FALSE, glm::value_ptr(value));
        CheckGLError("SetUniformMatrix4fv");
    }

    unsigned int RenderCommand::CreateBuffer()
    {
        unsigned int id = 0;
        glGenBuffers(1, &id);
        CheckGLError("CreateBuffer");
        return id;
    }

    void RenderCommand::destroyBuffer(unsigned int id)
    {
        assert(id && "Buffer is null!");
        glDeleteBuffers(1, &id);
        CheckGLError("destroyBuffer");
    }

    void RenderCommand::BindBuffer(unsigned int id, RenderProtocol::BufferType type)
    {
        assert(id && "Buffer is null!");
        glBindBuffer(GetBufferType(type), id);
        CheckGLError("BindBuffer");
    }

    void RenderCommand::UnbindBuffer(RenderProtocol::BufferType type)
    {
        glBindBuffer(GetBufferType(type), 0);
        CheckGLError("UnbindBuffer");
    }

    void RenderCommand::BufferData(RenderProtocol::BufferType type, 
        const void* data, unsigned int size, RenderProtocol::BufferUsage usage)
    {
        glBufferData(GetBufferType(type), size, data, GetBufferUsage(usage));
        CheckGLError("BufferData");
    }

    void RenderCommand::VertexAttribPointer(unsigned int index, unsigned int size, unsigned int stride, const void* pointer)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
        CheckGLError("VertexAttribPointer");
    }  

    void RenderCommand::DrawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count)
    {
        glDrawArrays(GetDrawMode(mode), first, count);
        CheckGLError("DrawArray");
    }

    void RenderCommand::DrawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices)
    {
        glDrawElements(GetDrawMode(mode), count, GetDrawElementType(type), indices);
        CheckGLError("DrawElements");
    }

    unsigned int RenderCommand::CreateVertexArray()
    {
        unsigned int id = 0;
        glGenVertexArrays(1, &id);
        CheckGLError("CreateVertexArray");
        return id;
    }

    void RenderCommand::destroyVertexArray(unsigned int id)
    {
        assert(id && "Vertex Array is null!");
        glDeleteVertexArrays(1, &id);
        CheckGLError("destroyVertexArray");
    }

    void RenderCommand::BindVertexArray(unsigned int id)
    {
        assert(id && "Vertex Array is null!");
        glBindVertexArray(id);
        CheckGLError("BindVertexArray");
    }

    void RenderCommand::UnbindVertexArray()
    {
        glBindVertexArray(0);
        CheckGLError("UnbindVertexArray");
    }
    
    int RenderCommand::GetMaxVertexAttribs()
    {
        int maxAttribs = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
        CheckGLError("GetMaxVertexAttribs");
        return maxAttribs;
    }

    unsigned int RenderCommand::CreateTexture()
    {
        unsigned int id = 0;
        glGenTextures(1, &id);
        CheckGLError("CreateTexture");
        return id;
    }

    void RenderCommand::destroyTexture(unsigned int id)
    {
        assert(id && "Texture is null!");
        glDeleteTextures(1, &id);
        CheckGLError("destroyTexture");
    }

    void RenderCommand::BindTexture(unsigned int id, RenderProtocol::TexType type)
    {
        assert(id && "Texture is null!");
        glBindTexture(GetTexType(type), id);
        CheckGLError("BindTexture");
    }

    void RenderCommand::UnbindTexture(RenderProtocol::TexType type)
    {
        glBindTexture(GetTexType(type), 0);
        CheckGLError("UnbindTexture");
    }

    void RenderCommand::SetTextureData(RenderProtocol::TexType type, int level, RenderProtocol::TexFormat internalFormat,
            int width, int height, RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data)
    {
        glTexImage2D(GetTexType(type), level, GetTexFormat(internalFormat), width, height,
            0, GetTexFormat(format), GetTexDataType(dataType), data);
        CheckGLError("SetTextureData");
    }

    void RenderCommand::GenerateMipmap(RenderProtocol::TexType type)
    {
        glGenerateMipmap(GetTexType(type));
        CheckGLError("GenerateMipmap");
    }

    void RenderCommand::ActivateTextureUnit(unsigned int unit)
    {
        assert(unit < 32 && "Texture unit out of range!");
        glActiveTexture(GL_TEXTURE0 + unit);
        CheckGLError("ActivateTextureUnit");
    }

    void RenderCommand::SetTextureWarpMode(RenderProtocol::TexType type, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround)
    {
        glTexParameteri(GetTexType(type), GetTexWrap(wrap), GetTexSurround(surround));
        CheckGLError("SetTextureWarpMode");
    }

    void RenderCommand::SetTextureFilterMode(RenderProtocol::TexType type, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType)
    {
        glTexParameteri(GetTexType(type), GetTexFilter(filter), GetTexFilterType(filterType));
        CheckGLError("SetTextureFilterMode");
    }
} // namespace Crash