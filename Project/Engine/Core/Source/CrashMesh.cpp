#include "CrashPCH.h"
#include "CrashMesh.h"

#include "CrashEngine.h"
#include "CrashRenderSystem.h"
#include "CrashRenderBuffer.h"
#include "CrashTexMgr.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Crash
{
    namespace
    {
        void _ProcessMesh(const std::string& modelPath, const aiScene *scene, aiMesh *mesh, SubMesh& subMesh)
        {
            std::vector<SubMesh::Vertex>&   vertices = subMesh.getVertices();
            std::vector<unsigned int>&      indices  = subMesh.getIndices();
            Material&                       material = subMesh.getMaterial();

            vertices.resize(mesh->mNumVertices);
            for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
            {
                SubMesh::Vertex& vertex = vertices[i];

                vertex.position = {
                    mesh->mVertices[i].x, 
                    mesh->mVertices[i].y, 
                    mesh->mVertices[i].z
                };

                vertex.normal = {
                    mesh->mNormals[i].x, 
                    mesh->mNormals[i].y, 
                    mesh->mNormals[i].z
                };
               
                if(mesh->mTextureCoords[0]) 
                {
                    vertex.texCoords = {
                        mesh->mTextureCoords[0][i].x, 
                        mesh->mTextureCoords[0][i].y
                    };
                }
            }

            for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
            {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; ++j)
                    indices.push_back(face.mIndices[j]);
            }

            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
            {
                aiString str = {};
                mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                if(str.C_Str() && str.C_Str()[0] != '\0')
                {
                    std::string texName = str.C_Str();
                    size_t pos = modelPath.find_last_of("\\/");
                    if(pos != std::string::npos)
                        texName = modelPath.substr(0, pos + 1) + texName;
                    
                    std::shared_ptr<Texture> texPtr = TexMgr::Instance()->createTexture(texName, RenderProtocol::TexType::Texture2D);
                    material.setDiffuseTex(texPtr);
                }
            }

            {
                aiString str = {};
                mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
                if(str.C_Str() && str.C_Str()[0] != '\0')
                {
                    std::string texName = str.C_Str();
                    size_t pos = modelPath.find_last_of("\\/");
                    if(pos != std::string::npos)
                        texName = modelPath.substr(0, pos + 1) + texName;

                    std::shared_ptr<Texture> texPtr = TexMgr::Instance()->createTexture(texName, RenderProtocol::TexType::Texture2D);
                    material.setSpecularTex(texPtr);
                }
            }
        }

        void _ProcessNode(const std::string& meshName, const aiScene *scene, aiNode *node, std::vector<SubMesh>& subMeshVec)
        {
            for(unsigned int i = 0; i < node->mNumChildren; ++i)
                _ProcessNode(meshName, scene, node->mChildren[i], subMeshVec);

            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                subMeshVec.emplace_back(mesh->mName.C_Str());
                _ProcessMesh(meshName, scene, mesh, subMeshVec.back());         
            }
        }

        void _LoadModel(Mesh& mesh)
        {
            const std::string& meshName= mesh.getName();
            const std::string fullPath = Engine::Instance()->getAssetPath() 
                + R"(Model/)" + meshName;

            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);    
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
            {
                LogManager::Instance()->log("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
                return;
            }

            _ProcessNode(meshName, scene, scene->mRootNode, mesh.getSubMeshes());
        }
    }

    Mesh::Mesh(const std::string& name)
        : mName(name)
    {
        _LoadModel(*this);
    }

    std::shared_ptr<Mesh> MeshMgr::createMesh(const std::string& name)
    {
        auto it = mMeshCache.find(name);
        if(it != mMeshCache.end())
            return it->second.lock();
        
        std::shared_ptr<Mesh> meshPtr(new Mesh(name), [](Mesh* mesh) {
                MeshMgr::Instance()->destroyMeshImpl(mesh); });

        mMeshCache[name] = meshPtr;

        return meshPtr;
    }

    void MeshMgr::destroyMeshImpl(Mesh* mesh)
    {
        mMeshCache.erase(mesh->getName());
        delete mesh;
    }
}