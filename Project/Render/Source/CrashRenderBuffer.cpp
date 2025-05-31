#include "CrashPCH.h"
#include "CrashRenderBuffer.h"
#include "CrashRenderCommand.h"

#include "CrashLogManager.h"

namespace Crash
{
    RenderBuffer::RenderBuffer(RenderProtocol::BufferType type,
        RenderProtocol::BufferUsage usage)
        : mType(type)
        , mUsage(usage)
    {

    }
    
    RenderBuffer::~RenderBuffer()
    {
        LogManager::Instance()->log("RenderBuffer destroy Buffer ID: " + std::to_string(mID));

        RenderCommand::destroyBuffer(mID);
    }

    void RenderBuffer::createHandle()
    {
        mID = RenderCommand::CreateBuffer();
    }

    void RenderBuffer::bind() const
    {
        RenderCommand::BindBuffer(mID, mType);
    }

    void RenderBuffer::unbind() const
    {
        RenderCommand::UnbindBuffer(mType);
    }

    void RenderBuffer::setBufferData(const void* data, unsigned int size)
    {
        bind();
        RenderCommand::BufferData(mType, data, size, mUsage);
    }

    VertexBuffer::VertexBuffer(RenderProtocol::BufferUsage usage)
        : RenderBuffer(RenderProtocol::BufferType::VertexBuffer, usage)
    {

    }

    IndexBuffer::IndexBuffer(RenderProtocol::BufferUsage usage)
        : RenderBuffer(RenderProtocol::BufferType::IndexBuffer, usage)
    {

    }

    VertexArrayObject::VertexArrayObject()
    {

    }

    VertexArrayObject::~VertexArrayObject()
    {
        RenderCommand::destroyVertexArray(mID);
    }

    void VertexArrayObject::createHandle()
    {
        mID = RenderCommand::CreateVertexArray();
    }

    void VertexArrayObject::bind() const
    {
        RenderCommand::BindVertexArray(mID);
    }

    void VertexArrayObject::unbind() const
    {
        RenderCommand::UnbindVertexArray();
    }

    void VertexArrayObject::addBuffer(const VertexBuffer* buffer, unsigned int index, unsigned int size, unsigned int stride, const void* pointer)
    {
        bind();
        buffer->bind();
        RenderCommand::VertexAttribPointer(index, size, stride, pointer);
    }

    void VertexArrayObject::addBuffer(const IndexBuffer* buffer)
    {
        bind();
        buffer->bind();
    }
}

