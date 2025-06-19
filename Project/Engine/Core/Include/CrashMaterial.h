#pragma once

#include "CrashEngineDefines.hpp"

namespace Crash
{
    class Texture;
    class ShaderProgram;
    class CRASH_ENGINE_API Material
    {
    public:

        void setAmbient(const glm::vec3& _v)                { mAmbient    = glm::vec4(_v, mAmbient.w);  };
        void setDiffuse(const glm::vec3& _v)                { mDiffuse    = glm::vec4(_v, mDiffuse.w);  };
        void setSpecular(const glm::vec3& _v)               { mSpecular   = glm::vec4(_v, mSpecular.w); };
        void setShininess(float _v)                         { mSpecular.w = _v; };

        void setDiffuseTex(const std::shared_ptr<Texture>& _tex)   { mDiffuseTex  = _tex; };
        void setSpecularTex(const std::shared_ptr<Texture>& _tex)  { mSpecularTex = _tex; };
        
        glm::vec3                           getAmbient()            const   { return glm::vec3(mAmbient); };
        glm::vec3                           getDiffuse ()           const   { return glm::vec3(mDiffuse); };
        glm::vec3                           getSpecular()           const   { return glm::vec3(mSpecular);};
        float                               getShininess(float _v)  const   { return mSpecular.w; };

        const std::shared_ptr<Texture>&     getDiffuseTex()         const   { return mDiffuseTex;  };
        const std::shared_ptr<Texture>&     getSpecularTex()        const   { return mSpecularTex; };

        void apply(const ShaderProgram* program) const;

        void reset();

    private:
        glm::vec4   mAmbient    = {0.f, 0.f, 0.f, 1.f};
        glm::vec4   mDiffuse    = {0.f, 0.f, 0.f, 1.f};
        glm::vec4   mSpecular   = {0.f, 0.f, 0.f, 1.f};     //  W:shininess

        std::shared_ptr<Texture>    mDiffuseTex;
        std::shared_ptr<Texture>    mSpecularTex;
    };
}