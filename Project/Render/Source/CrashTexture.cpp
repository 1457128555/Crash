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
    }

    void Texture::setTextureWarpMode(RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround)
    {
        bind();
        RenderCommand::SetTextureWarpMode(mType, wrap, surround);
    }

    void Texture::setTextureFilterMode(RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType)
    {
        bind();
        RenderCommand::SetTextureFilterMode(mType, filter, filterType);
    }
}