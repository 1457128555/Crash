#include "CrashPCH.h"
#include "CrashEntity.h"

#include "CrashEngine.h"
#include "CrashMesh.h"
#include "CrashRenderSystem.h"
#include "CrashRenderer.h"

namespace Crash
{
    SubEntity::SubEntity(SubMesh* subMeshPtr, Entity* entityPtr)
        : mSubMeshPtr(subMeshPtr)
        , mEntityPtr(entityPtr)
    {
        auto& vertices = mSubMeshPtr->getVertices();
        auto& indices  = mSubMeshPtr->getIndices();

        IndexBuffer* ibo = RenderSystem::Instance()->createIndexBuffer();
        RenderSystem::Instance()->setIndexBufferData(ibo, indices.data(), sizeof(indices[0]) * indices.size());

        VertexBuffer* vbo = RenderSystem::Instance()->createBuffer();
        RenderSystem::Instance()->setBufferData(vbo, vertices.data(), sizeof(vertices[0]) * vertices.size());

        mRenderOpt.bind();
        mRenderOpt.addIBO(ibo, true);
        mRenderOpt.addVBO(vbo, {
            { 0, 3, sizeof(SubMesh::Vertex), (const void*)offsetof(SubMesh::Vertex, position) },    // Vertex
            { 1, 3, sizeof(SubMesh::Vertex), (const void*)offsetof(SubMesh::Vertex, normal) },      // Normal
            { 2, 2, sizeof(SubMesh::Vertex), (const void*)offsetof(SubMesh::Vertex, texCoords) }    // TexCoords
        });
        mRenderOpt.unbind();

        mRenderOpt.setCount(static_cast<unsigned int>(indices.size()));
    }

    Material* SubEntity::getMaterial()     
    {
        return &mSubMeshPtr->getMaterial();
    }   

    ShaderProgram* SubEntity::getShaderProgram()     
    {
        return mEntityPtr->getShaderProgram().get();
    }

    glm::mat4* SubEntity::getWorldMatrix()    
    {
        return &mEntityPtr->getWorldMatrix();
    }   

    void SubEntity::setUniformObject()     
    {

    }
    
    Entity::Entity(const std::shared_ptr<Mesh>& meshPtr)
        : mMeshPtr(meshPtr)
    {   
        std::vector<SubMesh>& subMeshes = mMeshPtr->getSubMeshes();
        for(auto &&subMesh : subMeshes)
            mSubEntities.push_back(std::make_unique<SubEntity>(&subMesh, this));
    }

    void Entity::render()
    {
        for(auto &&subEntity : mSubEntities)
            Renderer::Instance()->renderSingleObject(subEntity.get());
    }
} // namespace Crash
