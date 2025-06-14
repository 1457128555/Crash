#pragma once

#include "CrashEngineDefines.hpp"
#include <vector>

namespace Crash
{
    class CRASH_ENGINE_API BasicGeometry
    {
    public:
        enum class DataType : unsigned int
        {
            Vertex      = 0x1<<0,
            Normal      = 0x1<<1,
            TexCoord    = 0x1<<2,
            Tangent     = 0x1<<3,
        };

        static bool HasFlag(BasicGeometry::DataType value, BasicGeometry::DataType flag)
        {
            return static_cast<unsigned int>(value) & static_cast<unsigned int>(flag);
        }

        static DataType ComFlag(std::vector<DataType> types)
        {
            unsigned int result = 0;
            for (auto t : types)
                result |= static_cast<unsigned int>(t);
            return static_cast<DataType>(result);
        }

        static void Triangle(DataType type, std::vector<float>& vertices);
        static void Triangle(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices);

        static void Quad(DataType type, std::vector<float>& vertices);
        static void Quad(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices);

        static void Cube(DataType type, std::vector<float>& vertices);
        static void Cube(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices);

        struct RenderPack
        {
            class VertexArrayObject*          vao = nullptr;
            class VertexBuffer*               vbo = nullptr;
            class IndexBuffer*                ibo = nullptr;
        };

        static RenderPack CreateRP(DataType type, std::vector<float>& vertices, std::vector<unsigned int>& indices);
        static RenderPack CreateTriangleRP(DataType type);
        static RenderPack CreateQuadRP(DataType type);
        static RenderPack CreateCubeRP(DataType type);

        static void DestoryRenderPack(RenderPack& pack);
    };
}