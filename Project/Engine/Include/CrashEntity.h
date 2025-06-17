#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashRenderable.h"
#include "CrashRenderOperation.h"

namespace Crash
{
    class Mesh;
    class SubMesh;
    class Entity;
    
    class CRASH_ENGINE_API SubEntity : public Renderable
    {
    public:
        SubEntity(SubMesh* subMeshPtr, Entity* entityPtr);
        ~SubEntity() = default;

        virtual Material*                   getMaterial()               override; 
        virtual ShaderProgram*              getShaderProgram()          override; 
        virtual glm::mat4*                  getWorldMatrix()            override; 
        virtual void                        setUniformObject()          override; 
        virtual RenderOperation*            getRenderOperation()        override { return &mRenderOpt; }

    private:
        Entity*     mEntityPtr  = nullptr;
        SubMesh*    mSubMeshPtr = nullptr;
        RenderOperation mRenderOpt;

    private:
        SubEntity(const SubEntity&)               = delete;
        SubEntity& operator=(const SubEntity&)    = delete;
    };

    class CRASH_ENGINE_API Entity
    {
    public:
        explicit Entity(const std::shared_ptr<Mesh>& meshPtr);
        ~Entity() = default;

        void setShaderProgram(const std::shared_ptr<ShaderProgram>& shaderPtr) { mShaderPtr = shaderPtr; }
        
        const std::shared_ptr<Mesh>&            getMesh()           { return mMeshPtr; };
        const std::shared_ptr<ShaderProgram>&   getShaderProgram()  { return mShaderPtr; }
        glm::mat4&                              getWorldMatrix()    { return mWorldMatrix; }; 
        
        void render();

    private:
        std::shared_ptr<Mesh> mMeshPtr;
        std::shared_ptr<ShaderProgram> mShaderPtr;
        std::vector<std::unique_ptr<SubEntity>> mSubEntities;

        glm::mat4 mWorldMatrix = glm::mat4(1.0f);

    private:
        Entity(const Entity&)               = delete;
        Entity& operator=(const Entity&)    = delete;
    };
}

