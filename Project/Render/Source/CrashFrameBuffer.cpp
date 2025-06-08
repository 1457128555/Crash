#include "CrashPCH.h"
#include "CrashFrameBuffer.h"
#include "CrashRenderCommand.h"

#include "CrashTexture.h"

namespace Crash
{
    FrameBuffer::FrameBuffer(const std::string& name,int width, int height, bool useRBO)
        : mName(name), mWidth(width), mHeight(height), mUseRBO(useRBO)
    {
        mColorAttachment = std::make_shared<Texture>(mName + "_FBO_Color_" + std::to_string(mID), RenderProtocol::TexType::Texture2D);
        if(!mUseRBO)
            mDepthStencilAttachment = std::make_shared<Texture>(mName + "_FBO_DepthStencil_" + std::to_string(mID), RenderProtocol::TexType::Texture2D);
    }

    FrameBuffer::~FrameBuffer()
    {
        if (mID != 0u)
        {
            RenderCommand::destroyFrameBuffer(mID);
            mID = 0u;
        }
        
        if(mRBO)
            RenderCommand::DestroyRenderBufferObject(mRBO);
    }

    void FrameBuffer::createHandle()
    {
        assert(mID == 0u && "FrameBuffer already created!");
        mID = RenderCommand::CreateFrameBuffer();

        bind();
        
        mColorAttachment->createHandle();
        mColorAttachment->setTextureData(0, RenderProtocol::TexFormat::RGBA, mWidth, mHeight,
            RenderProtocol::TexFormat::RGBA, RenderProtocol::TexDataType::UnsignedByte, nullptr, false);
        RenderCommand::SetFramebufferTexture2D(mID, RenderProtocol::AttachmentType::ColorAttachment0, mColorAttachment->getID());
        mColorAttachment->setTextureWarpMode(RenderProtocol::TexWrap::S, RenderProtocol::TexSurround::ClampToEdge);
        mColorAttachment->setTextureWarpMode(RenderProtocol::TexWrap::T, RenderProtocol::TexSurround::ClampToEdge);

        if(mUseRBO)
        {
            mRBO = RenderCommand::CreateRenderBufferObject();
            RenderCommand::BindRenderBufferObject(mRBO);
            RenderCommand::SetRenderBufferStorage(RenderProtocol::TexFormat::Depth24Stencil8, mWidth, mHeight);
            RenderCommand::SerFrameBufferRenderBuffer(mID, RenderProtocol::AttachmentType::DepthStencilAttachment, mRBO);
        }
        else
        {
            mDepthStencilAttachment->createHandle();
            mDepthStencilAttachment->setTextureData(0, RenderProtocol::TexFormat::Depth24Stencil8, mWidth, mHeight,
                RenderProtocol::TexFormat::DepthStencil, RenderProtocol::TexDataType::UnsignedInt24_8, nullptr, false);
            RenderCommand::SetFramebufferTexture2D(mID, RenderProtocol::AttachmentType::DepthStencilAttachment, mDepthStencilAttachment->getID());
            mDepthStencilAttachment->setTextureWarpMode(RenderProtocol::TexWrap::S, RenderProtocol::TexSurround::ClampToEdge);
            mDepthStencilAttachment->setTextureWarpMode(RenderProtocol::TexWrap::T, RenderProtocol::TexSurround::ClampToEdge);

        }

        if (!checkComplete())
        {
            assert(false && "FrameBuffer is not complete!");
            LogManager::Instance()->log("ERROR::FRAMEBUFFER:: " + mName + " FrameBuffer is not complete!");
        }
    }

    void FrameBuffer::bind() const
    {
        RenderCommand::BindFrameBuffer(mID);
    }

    void FrameBuffer::unbind() const
    {
        RenderCommand::UnbindFrameBuffer();
    }

    bool FrameBuffer::checkComplete() const
    {
        RenderCommand::BindFrameBuffer(mID);
        return RenderCommand::CheckFrameBufferComplete();
    }
}