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

        enum class AttachmentType : unsigned int
        {
            ColorAttachment0        = 0x1,
            ColorAttachment1        = 0x2,
            ColorAttachment2        = 0x3,
            ColorAttachment3        = 0x4,
            DepthAttachment         = 0x5,
            StencilAttachment       = 0x6,
            DepthStencilAttachment  = 0x7,
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
            
            Depth24Stencil8 = 0x8, // Special format for depth-stencil textures
            DepthStencil    = 0x9, // Generic depth-stencil format
        };

        enum class TexDataType : unsigned int
        {
            UnsignedByte    = 0x1,       
            UnsignedShort   = 0x2,       
            UnsignedInt     = 0x3,       
            Float           = 0x4,       
            UnsignedInt24_8 = 0x5, // Special type for depth24-stencil8 textures
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
                case TexFormat::Depth24Stencil8:    return 4;   // Depth24Stencil8 is typically 4 bytes
                case TexFormat::DepthStencil:       return 4;     // Generic depth-stencil format is also typically 4 bytes
                default:               break; // Invalid format
            }
            assert(false && "Invalid texture format");
            return 4; // Default to RGBA
        }

        enum class CompareFunc : unsigned int
        {
            Never          = 0x1,
            Always         = 0x2,
            Less           = 0x3,
            LessEqual      = 0x4,
            Equal          = 0x5,
            GreaterEqual   = 0x6,
            Greater        = 0x7,
            NotEqual       = 0x8,
        };

        enum class OperateFunc : unsigned int
        {
            Keep           = 0x1,
            Zero           = 0x2,
            Replace        = 0x3,
            Invert         = 0x4,
            Increment      = 0x5,
            Decrement      = 0x6,
            IncrementWrap  = 0x7,
            DecrementWrap  = 0x8,
        };

        enum class BlendFunc : unsigned int
        {
            Zero                    = 0x1,
            One                     = 0x2,
            SrcColor                = 0x3,
            OneMinusSrcColor        = 0x4,
            DstColor                = 0x5,
            OneMinusDstColor        = 0x6,
            SrcAlpha                = 0x7,
            OneMinusSrcAlpha        = 0x8,
            DstAlpha                = 0x9,
            OneMinusDstAlpha        = 0xA,
            ConstantColor           = 0xB,
            OneMinusConstantColor   = 0xC,
            ConstantAlpha           = 0xD,
            OneMinusConstantAlpha   = 0xE,
        };
    }; 
}