// #include "CrashPCH.h"
// #include "CrashModel.h"

// #include "CrashEngine.h"
// #include "CrashRenderSystem.h"

// #include "CrashMesh.h"
// #include "CrashFileSystem.h"
// #include "CrashTexMgr.h"

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

// namespace Crash
// {
//     namespace
//     {
//         void ProcessMesh(const std::string& modelPath, aiMesh *mesh, const aiScene *scene, std::vector<Mesh*>& ret)
//         {
//             std::vector<MeshVertex> vertices;
//             std::vector<unsigned int> indices;
//             Material material;

//             for(unsigned int i = 0; i < mesh->mNumVertices; i++)
//             {
//                 MeshVertex vertex;
//                 vertex.position = {
//                     mesh->mVertices[i].x, 
//                     mesh->mVertices[i].y, 
//                     mesh->mVertices[i].z};
                
//                 vertex.normal = {
//                     mesh->mNormals[i].x, 
//                     mesh->mNormals[i].y, 
//                     mesh->mNormals[i].z};
                
            
//                 if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
//                 {
//                     vertex.texCoords = {
//                         mesh->mTextureCoords[0][i].x, 
//                         mesh->mTextureCoords[0][i].y};
//                 }
//                 vertices.push_back(vertex);
//             }

//             for(unsigned int i = 0; i < mesh->mNumFaces; i++)
//             {
//                 aiFace face = mesh->mFaces[i];
//                 for(unsigned int j = 0; j < face.mNumIndices; j++)
//                     indices.push_back(face.mIndices[j]);
//             }

//             // Process materials
//             aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
//             {
//                 aiString str = {};
//                 auto count = mat->GetTextureCount(aiTextureType_DIFFUSE);
//                 mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
//                 if(str.C_Str() && str.C_Str()[0] != '\0')
//                 {
//                     std::string texName = str.C_Str();
//                     size_t pos = modelPath.find_last_of("\\/");
//                     if(pos != std::string::npos)
//                         texName = modelPath.substr(0, pos + 1) + texName;
                    
//                     std::shared_ptr<Texture> texPtr = TexMgr::Instance()->createTexture(texName, RenderProtocol::TexType::Texture2D);
//                     material.setDiffuseTex(texPtr);
//                 }
//             }

//             {
//                 aiString str = {};
//                 auto count = mat->GetTextureCount(aiTextureType_SPECULAR);
//                 mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
//                 if(str.C_Str() && str.C_Str()[0] != '\0')
//                 {
//                     std::string texName = str.C_Str();
//                     size_t pos = modelPath.find_last_of("\\/");
//                     if(pos != std::string::npos)
//                         texName = modelPath.substr(0, pos + 1) + texName;

//                     std::shared_ptr<Texture> texPtr = TexMgr::Instance()->createTexture(texName, RenderProtocol::TexType::Texture2D);
//                     material.setSpecularTex(texPtr);
//                 }
//             }

//             Mesh* newMesh = new Mesh(mesh->mName.C_Str());
//             newMesh->setMeshData(vertices, indices);
//             newMesh->setMaterial(material);
//             ret.push_back(newMesh);
//         }

//         void ProcessNode(const std::string& modelPath, aiNode *node, const aiScene *scene, std::vector<Mesh*>& ret)
//         {
//             for(unsigned int i = 0; i < node->mNumMeshes; i++)
//             {
//                 aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//                 ProcessMesh(modelPath, mesh, scene, ret);         
//             }

//             for(unsigned int i = 0; i < node->mNumChildren; i++)
//                 ProcessNode(modelPath, node->mChildren[i], scene, ret);
//         }

//         std::vector<Mesh*> LoadModel(const std::string& modelPath)
//         {
//             std::vector<Mesh*> ret;

//             const std::string fullModelPath =  Engine::Instance()->getAssetPath() + R"(Model/)" + modelPath;
//             Assimp::Importer importer;
//             const aiScene *scene = importer.ReadFile(fullModelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);    
            
//             if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
//             {
//                 LogManager::Instance()->log("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
//                 return ret;
//             }

//             ProcessNode(modelPath, scene->mRootNode, scene, ret);

//             return ret;
//         }
//     }

//     Model::Model(const std::string& name)
//         : mName(name)
//     {
//         mMeshes = LoadModel(name);
//     }

//     Model::~Model()
//     {
//         for(auto& mesh : mMeshes)
//             delete mesh;
//         mMeshes.clear();
//     }

//     void Model::draw(ShaderProgram* shader) const
//     {
//         for (const auto& mesh : mMeshes)
//         {
//             mesh->draw(shader);
//         }
//     }

// }