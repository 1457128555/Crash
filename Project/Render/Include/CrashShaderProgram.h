#pragma once

#include <string>
#include "CrashRenderProtocol.hpp"

namespace Crash
{
    class ShaderProgram
    {
        friend class RenderSystem;
    private:
        ShaderProgram(const std::string& name, const std::string& vs, const std::string& fs);
        ~ShaderProgram();

        void createHandle();

        void bind() const;

        void unbind() const;
    
        unsigned int getID() const { return mID; }

        void setUniform1i(const std::string& name, int value)                       const;
        void setUniform4f(const std::string& name, const glm::vec4& value)          const;
        void setUniformMatrix4fv(const std::string& name, const glm::mat4& value)   const;

        void setUniformBlockBinding(const std::string& blockName, unsigned int bindingPoint) const;
    private:
        const std::string mName;
        const std::string mVS;
        const std::string mFS;

        unsigned int mID;
    };
}