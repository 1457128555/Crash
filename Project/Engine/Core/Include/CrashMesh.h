#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashMaterial.h"

namespace Crash
{
    class CRASH_ENGINE_API SubMesh
    {
    public:
        struct Vertex
        {
            glm::vec3 position  =   { 0.f, 0.f, 0.f }; 
            glm::vec3 normal    =   { 0.f, 0.f, 0.f }; 
            glm::vec2 texCoords =   { 0.f, 0.f };      
        };

        explicit SubMesh(const std::string& name) : mName(name) {};
        ~SubMesh() = default;

        std::vector<Vertex>&        getVertices()   { return mVertices; }
        std::vector<unsigned int>&  getIndices()    { return mIndices; }
        Material&                   getMaterial()   { return mMaterial; }    

    private:
        const std::string           mName;         
        std::vector<Vertex>         mVertices;          
        std::vector<unsigned int>   mIndices;         
        Material                    mMaterial;     
    };

    class CRASH_ENGINE_API Mesh
    {
        friend class MeshMgr;
    public:
        const std::string&    getName()     const   {return mName;};
        std::vector<SubMesh>& getSubMeshes()        { return mSubMeshes; }

    private:
        explicit Mesh(const std::string& name);
        ~Mesh() = default;

    private:
        const std::string           mName; 
        std::vector<SubMesh>        mSubMeshes;          
    };

    class CRASH_ENGINE_API MeshMgr : public Singleton<MeshMgr>
    {
    public:
        std::shared_ptr<Mesh> createMesh(const std::string& name);
        void destroyMeshImpl(Mesh* mesh);

    private:
        std::unordered_map<std::string, std::weak_ptr<Mesh>> mMeshCache;
    };

    // class ShaderProgram;

    // struct MeshVertex
    // {
    //     glm::vec3 position  =   { 0.f, 0.f, 0.f };      // Position in 3D space
    //     glm::vec3 normal    =   { 0.f, 0.f, 0.f };      // Normal vector for lighting calculations
    //     glm::vec2 texCoords =   { 0.f, 0.f };           // Texture coordinates for mapping textures
    // };

    // class CRASH_ENGINE_API Mesh
    // {
    // public:
    //     explicit Mesh(const std::string& name = "Define Mesh");
    //     ~Mesh();

    //     void setMeshData(
    //         const std::vector<MeshVertex>& vertices, 
    //         const std::vector<unsigned int>& indices);

    //     void setMaterial(const Material& material)          { mMaterial = material; }
    //     const Material& getMaterial()               const   { return mMaterial; }

    //     void destroyRenderData();

    //     void draw(ShaderProgram* shader) const;

    // private:
    //     const std::string           mName = "Define Mesh";

    //     std::vector<MeshVertex>     mVertices;          // List of vertices
    //     std::vector<unsigned int>   mIndices;           // List of indices for indexed drawing
    //     Material                    mMaterial;    

    //     class VertexArrayObject*    mVAO = nullptr;          
    //     class VertexBuffer*         mVBO = nullptr;      
    //     class IndexBuffer*          mEBO = nullptr;     
    // };
}