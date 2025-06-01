#pragma once

#include "CrashRenderDefines.hpp"

namespace Crash
{
    class CRASH_RENDER_API RenderProtocol
    {
    public:
        enum class ClearFlag : unsigned int
        {
            Color   = 0x1,
            Depth   = 0x2,
            Stencil = 0x4,
            All     = Color | Depth | Stencil
        };

        enum class ShaderType : unsigned int
        {
            VertexShader   = 0x1,
            FragmentShader = 0x2,
            GeometryShader = 0x3,
            ComputeShader  = 0x4,
        };

        enum class BufferType : unsigned int
        {
            VertexBuffer   = 0x1,
            IndexBuffer    = 0x2,
        };

        enum class BufferUsage : unsigned int
        {
            StaticDraw     = 0x1,
            DynamicDraw    = 0x2,
            StreamDraw     = 0x3,
        };

        enum class DrawMode : unsigned int
        {
            Points         = 0x1,
            Lines          = 0x2,
            LineStrip      = 0x3,
            Triangles      = 0x4,
            TriangleStrip  = 0x5,
            TriangleFan    = 0x6,
        };

        enum class DrawElementType : unsigned int
        {
            UnsignedByte   = 0x1,
            UnsignedShort  = 0x2,
            UnsignedInt    = 0x3,
        };

        enum class PolygonMode : unsigned int
        {
            Fill           = 0x1,
            Line           = 0x2,
            Point          = 0x3,
        };

        enum class FaceMode : unsigned int
        {
            Front          = 0x1,
            Back           = 0x2,
            FrontAndBack   = 0x3,
        };

        enum class TexType : unsigned int
        {
            Texture1D      = 0x1,
            Texture2D      = 0x2,
            Texture3D      = 0x3,
            TextureCubeMap = 0x4,
        };

        enum class TexFormat : unsigned int
        {
            Red          = 0x1,       
            Green        = 0x2,       
            Blue         = 0x3,       
            Alpha        = 0x4,       
            RG           = 0x5,       
            RGB          = 0x6,       
            RGBA         = 0x7,       
        };

        enum class TexDataType : unsigned int
        {
            UnsignedByte   = 0x1,       
            UnsignedShort  = 0x2,       
            UnsignedInt    = 0x3,       
            Float          = 0x4,       
        };

        enum class TexSurround : unsigned int
        {
            Repeat         = 0x1,       
            MirroredRepeat = 0x2,      
            ClampToBorder  = 0x3,       
            ClampToEdge    = 0x4,       
        };

        enum class TexWrap : unsigned int
        {
            S              = 0x1,       
            T              = 0x2,       
            R              = 0x3,       
        };

        enum class TexFilter : unsigned int
        {
            MinFilter      = 0x1,       
            MagFilter      = 0x2,       
        };

        enum class TexFilterType : unsigned int
        {
            Nearest                 = 0x1,   
            Linear                  = 0x2,   
            NearestMipmapNearest    = 0x3,   
            LinearMipmapNearest     = 0x4,   
            NearestMipmapLinear     = 0x5,   
            LinearMipmapLinear      = 0x6,   
        };

        inline static int GetTexFormatByteSize(TexFormat format)
        {
            switch (format)
            {
                case TexFormat::Red:   return 1;
                case TexFormat::Green: return 1;
                case TexFormat::Blue:  return 1;
                case TexFormat::Alpha: return 1;
                case TexFormat::RG:    return 2;
                case TexFormat::RGB:   return 3;
                case TexFormat::RGBA:  return 4;
                default:               break; // Invalid format
            }
            assert(false && "Invalid texture format");
            return 4; // Default to RGBA
        }
    }; 
}