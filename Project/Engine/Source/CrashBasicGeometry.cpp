#include "CrashPCH.h"
#include "CrashBasicGeometry.h"

namespace Crash
{
    namespace
    {
        struct _Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 tangent;
        };
    }

    void BasicGeometry::Triangle(DataType type, std::vector<float>& vertices)
    {
        std::array<_Vertex, 3> triangle = {
            _Vertex{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },  // 顶点3（左下）
            _Vertex{ glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },  // 顶点2（右下）
            _Vertex{ glm::vec3(0.0f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) },  // 顶点1（上）
        };

        for(auto &&vertex : triangle)
        {
            if (HasFlag(type,DataType::Vertex))
            {
                vertices.push_back(vertex.position.x);
                vertices.push_back(vertex.position.y);
                vertices.push_back(vertex.position.z);
            }
            if (HasFlag(type,DataType::Normal))
            {
                vertices.push_back(vertex.normal.x);
                vertices.push_back(vertex.normal.y);
                vertices.push_back(vertex.normal.z);
            }
            if (HasFlag(type,DataType::TexCoord))
            {
                vertices.push_back(vertex.texCoord.x);
                vertices.push_back(vertex.texCoord.y);
            }
            if (HasFlag(type,DataType::Tangent))
            {
                vertices.push_back(vertex.tangent.x);
                vertices.push_back(vertex.tangent.y);
                vertices.push_back(vertex.tangent.z);
            }
        }
    }

    void BasicGeometry::Triangle(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {
        Triangle(type, vertices);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
    }

    void BasicGeometry::Quad(DataType type, std::vector<float>& vertices)
    {
        std::array<_Vertex, 6> quad = {
            _Vertex{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 左上
            _Vertex{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 左下
            _Vertex{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 右下
            _Vertex{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 左上
            _Vertex{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 右下
            _Vertex{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }  // 右上
        };

        for(auto &&vertex : quad)
        {
            if (HasFlag(type,DataType::Vertex))
            {
                vertices.push_back(vertex.position.x);
                vertices.push_back(vertex.position.y);
                vertices.push_back(vertex.position.z);
            }
            if (HasFlag(type,DataType::Normal))
            {
                vertices.push_back(vertex.normal.x);
                vertices.push_back(vertex.normal.y);
                vertices.push_back(vertex.normal.z);
            }
            if (HasFlag(type,DataType::TexCoord))
            {
                vertices.push_back(vertex.texCoord.x);
                vertices.push_back(vertex.texCoord.y);
            }
            if (HasFlag(type,DataType::Tangent))
            {
                vertices.push_back(vertex.tangent.x);
                vertices.push_back(vertex.tangent.y);
                vertices.push_back(vertex.tangent.z);
            }
        }
    }

    void BasicGeometry::Quad(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {
        std::array<_Vertex, 4> quadVertices = {
            _Vertex{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 左上 0
            _Vertex{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 左下 1
            _Vertex{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // 右下 2
            _Vertex{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }  // 右上 3
        };

        for(auto &&vertex : quadVertices)
        {
            if (HasFlag(type,DataType::Vertex))
            {
                vertices.push_back(vertex.position.x);
                vertices.push_back(vertex.position.y);
                vertices.push_back(vertex.position.z);
            }
            if (HasFlag(type,DataType::Normal))
            {
                vertices.push_back(vertex.normal.x);
                vertices.push_back(vertex.normal.y);
                vertices.push_back(vertex.normal.z);
            }
            if (HasFlag(type,DataType::TexCoord))
            {
                vertices.push_back(vertex.texCoord.x);
                vertices.push_back(vertex.texCoord.y);
            }
            if (HasFlag(type,DataType::Tangent))
            {
                vertices.push_back(vertex.tangent.x);
                vertices.push_back(vertex.tangent.y);
                vertices.push_back(vertex.tangent.z);
            }
        }

        std::array<unsigned int, 6> quadIndices = {
            0, 1, 2, 
            0, 2, 3  
        };

        indices.insert(indices.end(), quadIndices.begin(), quadIndices.end());
    }

    void BasicGeometry::Cube(DataType type, std::vector<float>& vertices)
    {
        // 立方体每个面独立顶点，逆时针顺序，适合背面剔除和法线贴图
        std::array<_Vertex, 36> cube = {
            // 后面 (-Z)
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(0,0,-1), glm::vec2(0,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(0,0,-1), glm::vec2(1,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(0,0,-1), glm::vec2(1,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(0,0,-1), glm::vec2(0,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(0,0,-1), glm::vec2(0,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(0,0,-1), glm::vec2(1,1), glm::vec3(1,0,0)},

            // 前面 (+Z)
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(0,0,1), glm::vec2(0,0), glm::vec3(-1,0,0)},
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(0,0,1), glm::vec2(1,0), glm::vec3(-1,0,0)},
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(0,0,1), glm::vec2(1,1), glm::vec3(-1,0,0)},
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(0,0,1), glm::vec2(0,0), glm::vec3(-1,0,0)},
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(0,0,1), glm::vec2(1,1), glm::vec3(-1,0,0)},
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(0,0,1), glm::vec2(0,1), glm::vec3(-1,0,0)},

            // 左面 (-X)
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(-1,0,0), glm::vec2(0,0), glm::vec3(0,0,-1)},
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(-1,0,0), glm::vec2(1,0), glm::vec3(0,0,-1)},
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(-1,0,0), glm::vec2(1,1), glm::vec3(0,0,-1)},
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(-1,0,0), glm::vec2(0,0), glm::vec3(0,0,-1)},
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(-1,0,0), glm::vec2(1,1), glm::vec3(0,0,-1)},
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(-1,0,0), glm::vec2(0,1), glm::vec3(0,0,-1)},

            // 右面 (+X)
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(1,0,0), glm::vec2(0,0), glm::vec3(0,0,1)},
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(1,0,0), glm::vec2(1,1), glm::vec3(0,0,1)},
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1)},
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(1,0,0), glm::vec2(0,0), glm::vec3(0,0,1)},
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(1,0,0), glm::vec2(0,1), glm::vec3(0,0,1)},
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(1,0,0), glm::vec2(1,1), glm::vec3(0,0,1)},

            // 下 (-Y)
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(0,-1,0), glm::vec2(0,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(0,-1,0), glm::vec2(1,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(0,-1,0), glm::vec2(1,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(0,-1,0), glm::vec2(0,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(0,-1,0), glm::vec2(1,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(0,-1,0), glm::vec2(0,0), glm::vec3(1,0,0)},

            // 上 (+Y)
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(0,1,0), glm::vec2(1,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(0,1,0), glm::vec2(1,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(1,0,0)},
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec3(1,0,0)},
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(0,1,0), glm::vec2(1,0), glm::vec3(1,0,0)}
        };

        for(auto &&vertex : cube)
        {
            if (HasFlag(type,DataType::Vertex))
            {
                vertices.push_back(vertex.position.x);
                vertices.push_back(vertex.position.y);
                vertices.push_back(vertex.position.z);
            }
            if (HasFlag(type,DataType::Normal))
            {
                vertices.push_back(vertex.normal.x);
                vertices.push_back(vertex.normal.y);
                vertices.push_back(vertex.normal.z);
            }
            if (HasFlag(type,DataType::TexCoord))
            {
                vertices.push_back(vertex.texCoord.x);
                vertices.push_back(vertex.texCoord.y);
            }
            if (HasFlag(type,DataType::Tangent))
            {
                vertices.push_back(vertex.tangent.x);
                vertices.push_back(vertex.tangent.y);
                vertices.push_back(vertex.tangent.z);
            }
        }
    }

    void BasicGeometry::Cube(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {
        // 立方体24顶点+36索引，逆时针顺序，适合背面剔除
        std::array<_Vertex, 24> cubeVertices = {
            // 前面 (+Z)
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(0,0,1), glm::vec2(0,0), glm::vec3(1,0,0)}, // 0
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(0,0,1), glm::vec2(1,0), glm::vec3(1,0,0)}, // 1
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(0,0,1), glm::vec2(1,1), glm::vec3(1,0,0)}, // 2
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(0,0,1), glm::vec2(0,1), glm::vec3(1,0,0)}, // 3
            // 后面 (-Z)
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(0,0,-1), glm::vec2(0,0), glm::vec3(-1,0,0)}, // 4
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(0,0,-1), glm::vec2(1,0), glm::vec3(-1,0,0)}, // 5
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(0,0,-1), glm::vec2(1,1), glm::vec3(-1,0,0)}, // 6
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(0,0,-1), glm::vec2(0,1), glm::vec3(-1,0,0)}, // 7
            // 左面 (-X)
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(-1,0,0), glm::vec2(0,0), glm::vec3(0,0,-1)}, // 8
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(-1,0,0), glm::vec2(1,0), glm::vec3(0,0,-1)}, // 9
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(-1,0,0), glm::vec2(1,1), glm::vec3(0,0,-1)}, //10
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(-1,0,0), glm::vec2(0,1), glm::vec3(0,0,-1)}, //11
            // 右面 (+X)
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(1,0,0), glm::vec2(0,0), glm::vec3(0,0,1)}, //12
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1)}, //13
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(1,0,0), glm::vec2(1,1), glm::vec3(0,0,1)}, //14
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(1,0,0), glm::vec2(0,1), glm::vec3(0,0,1)}, //15
            // 上面 (+Y)
            _Vertex{glm::vec3(-1, 1, 1), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec3(1,0,0)}, //16
            _Vertex{glm::vec3( 1, 1, 1), glm::vec3(0,1,0), glm::vec2(1,0), glm::vec3(1,0,0)}, //17
            _Vertex{glm::vec3( 1, 1,-1), glm::vec3(0,1,0), glm::vec2(1,1), glm::vec3(1,0,0)}, //18
            _Vertex{glm::vec3(-1, 1,-1), glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(1,0,0)}, //19
            // 下面 (-Y)
            _Vertex{glm::vec3(-1,-1,-1), glm::vec3(0,-1,0), glm::vec2(0,0), glm::vec3(1,0,0)}, //20
            _Vertex{glm::vec3( 1,-1,-1), glm::vec3(0,-1,0), glm::vec2(1,0), glm::vec3(1,0,0)}, //21
            _Vertex{glm::vec3( 1,-1, 1), glm::vec3(0,-1,0), glm::vec2(1,1), glm::vec3(1,0,0)}, //22
            _Vertex{glm::vec3(-1,-1, 1), glm::vec3(0,-1,0), glm::vec2(0,1), glm::vec3(1,0,0)}  //23
        };

        for(auto &&vertex : cubeVertices)
        {
            if (HasFlag(type,DataType::Vertex))
            {
                vertices.push_back(vertex.position.x);
                vertices.push_back(vertex.position.y);
                vertices.push_back(vertex.position.z);
            }
            if (HasFlag(type,DataType::Normal))
            {
                vertices.push_back(vertex.normal.x);
                vertices.push_back(vertex.normal.y);
                vertices.push_back(vertex.normal.z);
            }
            if (HasFlag(type,DataType::TexCoord))
            {
                vertices.push_back(vertex.texCoord.x);
                vertices.push_back(vertex.texCoord.y);
            }
            if (HasFlag(type,DataType::Tangent))
            {
                vertices.push_back(vertex.tangent.x);
                vertices.push_back(vertex.tangent.y);
                vertices.push_back(vertex.tangent.z);
            }
        }

        std::array<unsigned int, 36> cubeIndices = {
            // 前面
            0, 1, 2, 0, 2, 3,
            // 后面
            4, 5, 6, 4, 6, 7,
            // 左面
            8, 9,10, 8,10,11,
            // 右面
           12,13,14,12,14,15,
            // 上面
           16,17,18,16,18,19,
            // 下面
           20,21,22,20,22,23
        };
        indices.insert(indices.end(), cubeIndices.begin(), cubeIndices.end());
    }
}