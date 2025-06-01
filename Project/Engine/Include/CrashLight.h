#pragma once

#include "CrashEngineDefines.hpp"

namespace Crash
{
    class ShaderProgram;
    class CRASH_ENGINE_API Light
    {
    public:
        void setAmbient(const glm::vec3& ambient)                   { mAmbient  = glm::vec4(ambient, mAmbient.w); }
        void setDiffuse(const glm::vec3& diffuse)                   { mDiffuse  = glm::vec4(diffuse, mDiffuse.w); }
        void setSpecular(const glm::vec3& specular)                 { mSpecular = glm::vec4(specular, mSpecular.w); }

        const glm::vec4& getAmbient()                       const   { return mAmbient; }
        const glm::vec4& getDiffuse()                       const   { return mDiffuse; }
        const glm::vec4& getSpecular()                      const   { return mSpecular; }

        virtual void apply(const ShaderProgram* program)    const;

        virtual std::string getType()const = 0;
    protected:
        glm::vec4 mAmbient  = { 0.f, 0.f, 0.f, 1.f };
        glm::vec4 mDiffuse  = { 0.f, 0.f, 0.f, 1.f };
        glm::vec4 mSpecular = { 0.f, 0.f, 0.f, 1.f }; 
    };


    class CRASH_ENGINE_API PointLight : public Light
    {
    public:
        void setPosition(const glm::vec3& position)                 { mPosition = glm::vec4(position, mPosition.w); }
        const glm::vec4& getPosition()                      const   { return mPosition; }
        virtual void apply(const ShaderProgram* program)    const   override;
        virtual std::string getType()                       const   override { return "uPointLight"; }

    private:    
        glm::vec4 mPosition      = { 0.f, 0.f, 0.f, 1.f }; 
        glm::vec4 mAttenuation   = { 1.f, 0.09f, 0.032f, 0.f}; 
    };

    class CRASH_ENGINE_API DirLight : public Light
    {
    public:
        void setDirection(const glm::vec3& direction)               { mDirection = glm::vec4(direction, mDirection.w); }
        const glm::vec4& getDirection()                     const   { return mDirection; }
        virtual void apply(const ShaderProgram* program)    const   override;
        virtual std::string getType()                       const   override { return "uDirLight"; }

    private:
        glm::vec4 mDirection = { -0.333f, -0.333f, -0.333f, 0.f }; 
    };

    class CRASH_ENGINE_API SpotLight : public Light
    {
    public:
        void setPosition(const glm::vec3& position)                 { mPosition = glm::vec4(position, mPosition.w); }
        void setDirection(const glm::vec3& direction)               { mDirection = glm::vec4(direction, mDirection.w); }

        const glm::vec4& getPosition()                      const   { return mPosition; }
        const glm::vec4& getDirection()                     const   { return mDirection; }

        virtual void apply(const ShaderProgram* program)    const   override;
        virtual std::string getType()                       const   override { return "uSpotLight"; }
    private:
        glm::vec4 mPosition         = { 0.f, 0.f, 0.f, 1.f };      
        glm::vec4 mDirection        = { 0.f, -1.f, 0.f, 0.f };   
        glm::vec4 mAttenuation      = { 1.f, 0.09f, 0.032f, 0.f}; 
        glm::vec4 mCutOff           = { glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), 0.f, 0.f }; 
    };

}