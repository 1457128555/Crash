#include "CrashPCH.h"

#include "CrashRenderOperation.h"
#include "CrashRenderSystem.h"

namespace Crash
{
    RenderOperation::RenderOperation() : mVAO(nullptr), mIBO(nullptr)
    {
        mVAO = RenderSystem::Instance()->createVertexArray();
    }

    RenderOperation::~RenderOperation()
    {
        RenderSystem::Instance()->destroyVertexArray(mVAO);
        if (mIBO)
            RenderSystem::Instance()->destroyIndexBuffer(mIBO);

        clearVBO(true);
    }

    void RenderOperation::bind()     const
    {
        RenderSystem::Instance()->bindVertexArray(mVAO);
    }

    void RenderOperation::unbind()   const
    {
        RenderSystem::Instance()->unbindVertexArray();
    }

    void RenderOperation::addIBO(IndexBuffer* ibo, bool destoryHold)
    {
        if(destoryHold && mIBO)
            RenderSystem::Instance()->destroyIndexBuffer(mIBO);
        mIBO = ibo;
        RenderSystem::Instance()->addBufferToVertexArray(mVAO, mIBO);
    }

    void RenderOperation::addVBO(VertexBuffer* vbo, const std::vector<VBO_DESC>& descVec, unsigned int insStep )
    {
        mVBOs.push_back(vbo);
        for(const auto& desc : descVec)
            RenderSystem::Instance()->addBufferToVertexArray(mVAO, vbo, desc.index, desc.size, desc.stride, desc.pointer, insStep);
    }

    void RenderOperation::clearVBO(bool destoryHold)
    {
        if(destoryHold)
        {
            for(auto vbo : mVBOs)
                RenderSystem::Instance()->destroyBuffer(vbo);
        }

       mVBOs.clear();
    }
}