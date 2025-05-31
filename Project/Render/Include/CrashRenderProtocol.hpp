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
            Red          = 0x1,       //  红色通道
            Green        = 0x2,       //  绿色通道
            Blue         = 0x3,       //  蓝色通道
            Alpha        = 0x4,       //  透明度通道
            RG           = 0x5,       //  红色和绿色通道
            RGB          = 0x6,       //  红色、绿色和蓝色通道
            RGBA         = 0x7,       //  红色、绿色、蓝色和透明度通道
        };

        enum class TexDataType : unsigned int
        {
            UnsignedByte   = 0x1,       //  无符号字节
            UnsignedShort  = 0x2,       //  无符号短整型
            UnsignedInt    = 0x3,       //  无符号整型
            Float          = 0x4,       //  浮点型
        };

        enum class TexSurround : unsigned int
        {
            Repeat         = 0x1,       //  对纹理的默认行为。重复纹理图像。
            MirroredRepeat = 0x2,       //  和GL_REPEAT一样，但每次重复图片是镜像放置的。
            ClampToBorder  = 0x3,       //  超出的坐标为用户指定的边缘颜色。
            ClampToEdge    = 0x4,       //  纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
        };

        enum class TexWrap : unsigned int
        {
            S              = 0x1,       //  水平纹理坐标    X
            T              = 0x2,       //  垂直纹理坐标    Y
            R              = 0x3,       //  深度纹理坐标    Z
        };

        enum class TexFilter : unsigned int
        {
            MinFilter      = 0x1,       //  纹理缩小过滤器
            MagFilter      = 0x2,       //  纹理放大过滤器
        };

        enum class TexFilterType : unsigned int
        {
            Nearest                 = 0x1,   //  最近点过滤器
            Linear                  = 0x2,   //  双线性过滤器
            NearestMipmapNearest    = 0x3,   //  最近点mipmap过滤器
            LinearMipmapNearest     = 0x4,   //  双线性mipmap过滤器
            NearestMipmapLinear     = 0x5,   //  最近点mipmap线性过滤器
            LinearMipmapLinear      = 0x6,   //  双线性mipmap线性过滤器
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