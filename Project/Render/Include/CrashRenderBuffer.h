#pragma once

#include "CrashRenderProtocol.hpp"

namespace Crash
{   
     class RenderBuffer
     {
        friend class RenderSystem;
    protected:
        RenderBuffer(RenderProtocol::BufferType type, RenderProtocol::BufferUsage usage);
        virtual ~RenderBuffer();

        void createHandle();

        unsigned int getID()                            const { return mID; }
        RenderProtocol::BufferType getBufferType()      const { return mType; }
        RenderProtocol::BufferUsage getBufferUsage()    const { return mUsage; }

        void bind()   const;
        void unbind() const;
        void bindBase(unsigned int bindingPoint) const;
        void setBufferData(const void* data, unsigned int size);
        void setBufferSubData(unsigned int offset, const void* data, unsigned int size);

    private:
        const RenderProtocol::BufferType  mType;
        const RenderProtocol::BufferUsage mUsage;
        unsigned int mID = 0u;

    private:
        RenderBuffer(RenderBuffer&)                     = delete;
        RenderBuffer& operator=(const RenderBuffer&)    = delete;
     };

     class VertexBuffer : public RenderBuffer
     {
        friend class RenderSystem;
        friend class VertexArrayObject;
    private:
        explicit VertexBuffer(RenderProtocol::BufferUsage usage);
        virtual ~VertexBuffer() override = default;
     };

    class IndexBuffer : public RenderBuffer
    {
        friend class RenderSystem;
        friend class VertexArrayObject;
    private:
        explicit IndexBuffer(RenderProtocol::BufferUsage usage);
        virtual ~IndexBuffer() override = default;
    };

    class UniformBuffer : public RenderBuffer
    {
        friend class RenderSystem;
    private:
        explicit UniformBuffer(RenderProtocol::BufferUsage usage);
        virtual ~UniformBuffer() override = default;
    };

    class VertexArrayObject
    {
        friend class RenderSystem;
    private:
        VertexArrayObject();
        virtual ~VertexArrayObject();

        void createHandle();

        void bind()   const;
        void unbind() const;

        void addBuffer(const IndexBuffer* buffer);
        void addBuffer(const VertexBuffer* buffer, unsigned int index, unsigned int size, 
            unsigned int stride, const void* pointer, unsigned int insStep);
    
    private:    
        unsigned int mID = 0u;

    private:
        VertexArrayObject(VertexArrayObject&)                     = delete;
        VertexArrayObject& operator=(const VertexArrayObject&)    = delete;
    };
}
