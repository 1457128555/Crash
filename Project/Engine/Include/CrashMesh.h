#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashMaterial.h"

namespace Crash
{
    class ShaderProgram;

    struct MeshVertex
    {
        glm::vec3 position  =   { 0.f, 0.f, 0.f };      // Position in 3D space
        glm::vec3 normal    =   { 0.f, 0.f, 0.f };      // Normal vector for lighting calculations
        glm::vec2 texCoords =   { 0.f, 0.f };           // Texture coordinates for mapping textures
    };

    class CRASH_ENGINE_API Mesh
    {
    public:
        explicit Mesh(const std::string& name = "Define Mesh");
        ~Mesh();

        void setMeshData(
            const std::vector<MeshVertex>& vertices, 
            const std::vector<unsigned int>& indices);

        void setMaterial(const Material& material)          { mMaterial = material; }
        const Material& getMaterial()               const   { return mMaterial; }

        void destroyRenderData();

        void draw(ShaderProgram* shader) const;

    private:
        const std::string           mName = "Define Mesh";

        std::vector<MeshVertex>     mVertices;          // List of vertices
        std::vector<unsigned int>   mIndices;           // List of indices for indexed drawing
        Material                    mMaterial;    

        class VertexArrayObject*    mVAO = nullptr;          
        class VertexBuffer*         mVBO = nullptr;      
        class IndexBuffer*          mEBO = nullptr;     
    };
}