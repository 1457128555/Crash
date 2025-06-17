#include "CrashPCH.h"   
#include "CrashShaderProgram.h"
#include "CrashRenderCommand.h"

namespace Crash
{
    namespace
    {
        class Shader
        {
        public:
            Shader(RenderProtocol::ShaderType type, const std::string& source)
                : mType(type)
#ifdef _WIN32
                , mSource("#version 330 core\n" + source)
#else
                , mSource("#version 300 es\nprecision highp float;\n" + source)
#endif
                , mID(RenderCommand::CreateShader(type, mSource))
            {

            }
            
            ~Shader()
            {
                RenderCommand::destroyShader(mID);
            }

            unsigned int getID()                 const   { return mID; }
            RenderProtocol::ShaderType getType() const   { return mType; }
        private:
            const RenderProtocol::ShaderType mType;
            const std::string mSource;
            unsigned int mID;
        private:
            Shader(const Shader&)               = delete;
            Shader& operator=(const Shader&)    = delete;
        };
    }

    ShaderProgram::ShaderProgram(const std::string& name, 
        const std::string& vs, 
        const std::string& fs,
        const std::string& gs)
        : mName(name)
        , mVS(vs)
        , mFS(fs)
        , mGS(gs)
        , mID(0)
    {
       
    }

    void ShaderProgram::createHandle()
    {
        Shader vertexShader(RenderProtocol::ShaderType::VertexShader, mVS);
        Shader fragmentShader(RenderProtocol::ShaderType::FragmentShader, mFS);
        std::vector<unsigned int> shaderIDs = { vertexShader.getID(), fragmentShader.getID() };

        if(!mGS.empty())
        {
            Shader geometryShader(RenderProtocol::ShaderType::GeometryShader, mGS);
            mID = RenderCommand::CreateShaderProgram({
                vertexShader.getID(),
                fragmentShader.getID(),
                geometryShader.getID(),
            });
        }
        else
        {
            mID = RenderCommand::CreateShaderProgram({
                vertexShader.getID(),
                fragmentShader.getID(),
            });       
         }
    }

    ShaderProgram::~ShaderProgram()
    {
        RenderCommand::destroyShaderProgram(mID);

    }

    void ShaderProgram::bind() const
    {   
        RenderCommand::BindShaderProgram(mID);
    }

    void ShaderProgram::unbind() const
    {
        RenderCommand::UnbindShaderProgram();
    }

    void ShaderProgram::setUniform1i(const std::string& name, int value) const
    {
        bind();
        int location = RenderCommand::GetUniformLocation(mID, name);
        if (location != -1)
            RenderCommand::SetUniform1i(location, value);
    }
    
    void ShaderProgram::setUniform4f(const std::string& name, const glm::vec4& value) const
    {
        bind();
        int location = RenderCommand::GetUniformLocation(mID, name);
        if (location != -1)
            RenderCommand::SetUniform4f(location, value);
    }
    
    void ShaderProgram::setUniformMatrix4fv(const std::string& name, const glm::mat4& value) const
    {
        bind();
        int location = RenderCommand::GetUniformLocation(mID, name);
        if (location != -1)
            RenderCommand::SetUniformMatrix4fv(location, value);
    }

    void ShaderProgram::setUniformBlockBinding(const std::string& blockName, unsigned int bindingPoint) const
    {
        bind();
        int blockIndex = RenderCommand::GetUniformBlockIndex(mID, blockName);
        if (blockIndex != -1)
            RenderCommand::SetUniformBlockBinding(mID, blockIndex, bindingPoint);
    }
}