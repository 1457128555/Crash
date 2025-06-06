#include "CrashPCH.h"
#include "CrashMesh.h"

#include "CrashRenderSystem.h"

namespace Crash
{
    Mesh::Mesh(const std::string& name)
        : mName(name)
    {

    }

    Mesh::~Mesh()
    {
        destroyRenderData();
    }

    void Mesh::setMeshData(
        const std::vector<MeshVertex>& vertices, 
        const std::vector<unsigned int>& indices)
    {
        destroyRenderData();

        mVertices   = vertices;
        mIndices    = indices;

        mVAO = RenderSystem::Instance()->createVertexArray();
        mVBO = RenderSystem::Instance()->createBuffer();
        mEBO = RenderSystem::Instance()->createIndexBuffer();

        RenderSystem::Instance()->bindVertexArray(mVAO);
        RenderSystem::Instance()->setBufferData(mVBO, mVertices.data(), static_cast<unsigned int>(mVertices.size() * sizeof(MeshVertex)));

        RenderSystem::Instance()->addBufferToVertexArray(mVAO, mVBO, 0, sizeof(MeshVertex::position)  / sizeof(float), sizeof(MeshVertex), (void*)offsetof(MeshVertex, position));
        RenderSystem::Instance()->addBufferToVertexArray(mVAO, mVBO, 1, sizeof(MeshVertex::normal)    / sizeof(float), sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
        RenderSystem::Instance()->addBufferToVertexArray(mVAO, mVBO, 2, sizeof(MeshVertex::texCoords) / sizeof(float), sizeof(MeshVertex), (void*)offsetof(MeshVertex, texCoords));

        RenderSystem::Instance()->setIndexBufferData(mEBO, mIndices.data(), static_cast<unsigned int>(mIndices.size() * sizeof(unsigned int)));
        RenderSystem::Instance()->addBufferToVertexArray(mVAO, mEBO);

        RenderSystem::Instance()->unbindVertexArray();
    }

    void Mesh::destroyRenderData()
    {
        if (mVBO)   
            RenderSystem::Instance()->destroyBuffer(mVBO);        
        if (mEBO)
            RenderSystem::Instance()->destroyIndexBuffer(mEBO);
        if (mVAO)
            RenderSystem::Instance()->destroyVertexArray(mVAO);

        mVBO = nullptr;
        mEBO = nullptr;
        mVAO = nullptr;
    }

    void Mesh::draw(ShaderProgram* shader) const
    {

        if (!mVAO || mVertices.empty() || mIndices.empty() || !shader)
            return;

        RenderSystem::Instance()->bindVertexArray(mVAO);

        mMaterial.apply(shader);

        RenderSystem::Instance()->drawElements(RenderProtocol::DrawMode::Triangles, 
            static_cast<unsigned int>(mIndices.size()), RenderProtocol::DrawElementType::UnsignedInt, 0);
        
        RenderSystem::Instance()->unbindVertexArray();
    }
}