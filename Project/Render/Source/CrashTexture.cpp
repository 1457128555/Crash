#include "CrashTexture.h"
#include "CrashRenderCommand.h"

namespace Crash
{
    Texture::Texture(const std::string& name, RenderProtocol::TexType type)
        : mName(name) 
        , mType(type)
    {
        
    }

    void Texture::createHandle() 
    {
        mID = RenderCommand::CreateTexture();

        bind();
        RenderCommand::SetTextureWarpMode(mType, RenderProtocol::TexWrap::S, RenderProtocol::TexSurround::Repeat);
        RenderCommand::SetTextureWarpMode(mType, RenderProtocol::TexWrap::T, RenderProtocol::TexSurround::Repeat);
        RenderCommand::SetTextureFilterMode(mType, RenderProtocol::TexFilter::MinFilter, RenderProtocol::TexFilterType::Linear);
        RenderCommand::SetTextureFilterMode(mType, RenderProtocol::TexFilter::MagFilter, RenderProtocol::TexFilterType::Linear);
        unbind();
    }

    Texture::~Texture()
    {
        RenderCommand::destroyTexture(mID);
    }

    void Texture::bind() const
    {
        RenderCommand::BindTexture(mID, mType);
    }

    void Texture::unbind() const
    {
        RenderCommand::UnbindTexture(mType);
    }

    void Texture::setTextureData(int level, RenderProtocol::TexFormat internalFormat,
        int width, int height, RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data, bool generateMipmap)
    {
        mWidth = width;
        mHeight = height;
        mFormat = internalFormat;
        mGenerateMipmap = generateMipmap;

        bind();
        RenderCommand::SetTextureData(mType, level, mFormat, mWidth, mHeight, format, dataType, data);
        if (mGenerateMipmap)
            RenderCommand::GenerateMipmap(mType);
        unbind();
    }

    void Texture::setTextureWarpMode(RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround)
    {
        bind();
        RenderCommand::SetTextureWarpMode(mType, wrap, surround);
        unbind();
    }

    void Texture::setTextureFilterMode(RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType)
    {
        bind();
        RenderCommand::SetTextureFilterMode(mType, filter, filterType);
        unbind();
    }
}







// {
//     namespace
//     {
//         static const std::string _TextureHead = R"(Textures/)";

//         RenderProtocol::TexFormat ChannelsToTexFormat(int channels)
//         {
//             switch (channels)
//             {
//                 case 1: return RenderProtocol::TexFormat::Red;
//                 //case 2: return RenderProtocol::TexFormat::RG;
//                 case 3: return RenderProtocol::TexFormat::RGB;
//                 case 4: return RenderProtocol::TexFormat::RGBA;
//                 default: return RenderProtocol::TexFormat::RGBA; // 默认安全值
//             }
//             return RenderProtocol::TexFormat::RGBA; // 默认安全值
//         }
//     }



//     Texture2D::Texture2D(const std::string& texName)
//         : Texture(RenderProtocol::TexType::Texture2D)
//         , mTexName(texName)
//     {
//         int channels = 0;
//         unsigned char* data = FileSystem::ReadImage(_TextureHead + mTexName, &mWidth, &mHeight, &channels);
//         if (data)
//         {
//             mFormat = ChannelsToTexFormat(channels);

//             bind();
//             Render::Instance()->setTextureData(mType, 0, mFormat, mWidth, mHeight,
//                 mFormat, RenderProtocol::TexDataType::UnsignedByte, data);
//             Render::Instance()->generateMipmap(mType);
//             unbind();

//             FileSystem::FreeImage(data);
//         }
//     }
// }