#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashMaterial.h"

namespace Crash
{
    class Material;
    class RenderOperation;
    class ShaderProgram;

    class CRASH_ENGINE_API Renderable
    {
    public:
        virtual Material*                   getMaterial()               {return &mDefaultMaterial;}; 
        virtual RenderOperation*            getRenderOperation()        = 0;
        virtual ShaderProgram*              getShaderProgram()          = 0;
        virtual glm::mat4*                  getWorldMatrix()            {return &mIdentityMatrix;};
        virtual void                        setUniformObject()          {};

    private:
        inline static glm::mat4 mIdentityMatrix = glm::mat4(1.0f);
        inline static Material  mDefaultMaterial;
    };
}