#include "CrashPCH.h"

#include "CrashRenderOperation.h"
#include "CrashRenderSystem.h"

namespace Crash
{
    RenderOperation::RenderOperation() : mVAO(nullptr), mVBO(nullptr), mIBO(nullptr)
    {
        mVAO = RenderSystem::Instance()->createVertexArray();
    }

    RenderOperation::~RenderOperation()
    {
        RenderSystem::Instance()->destroyVertexArray(mVAO);
        if (mVBO)
            RenderSystem::Instance()->destroyBuffer(mVBO);
        if (mIBO)
            RenderSystem::Instance()->destroyIndexBuffer(mIBO);
    }

    void RenderOperation::bind()     const
    {
        RenderSystem::Instance()->bindVertexArray(mVAO);
    }

    void RenderOperation::unbind()   const
    {
        RenderSystem::Instance()->unbindVertexArray();
    }

    void RenderOperation::setIBO(IndexBuffer* ibo, bool destoryHold)
    {
        if(destoryHold && mIBO)
            RenderSystem::Instance()->destroyIndexBuffer(mIBO);
        mIBO = ibo;
        RenderSystem::Instance()->addBufferToVertexArray(mVAO, mIBO);
    }

    void RenderOperation::setVBO(VertexBuffer* vbo, const std::vector<VBO_DESC>& descVec, bool destoryHold)
    {
        if(destoryHold && mVBO)
            RenderSystem::Instance()->destroyBuffer(mVBO);
        mVBO = vbo;
        for(const auto& desc : descVec)
            RenderSystem::Instance()->addBufferToVertexArray(mVAO, mVBO, desc.index, desc.size, desc.stride, desc.pointer);
    }

    void RenderOperation::clear(bool destoryHold)
    {
        RenderSystem::Instance()->destroyVertexArray(mVAO);
        mVAO = RenderSystem::Instance()->createVertexArray();

        if(destoryHold)
        {
            if (mVBO)
                RenderSystem::Instance()->destroyBuffer(mVBO);
            if (mIBO)
                RenderSystem::Instance()->destroyIndexBuffer(mIBO);
        }

        mVBO = nullptr;
        mIBO = nullptr;
    }
}