#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashRenderBuffer.h"

namespace Crash
{
    class CRASH_ENGINE_API RenderOperation
    {
    public:
        RenderOperation();
        virtual ~RenderOperation();

        void bind()     const;
        void unbind()   const;
        
        void setIBO(IndexBuffer* ibo, bool destoryHold = true);

        struct VBO_DESC
        {
            unsigned int index = 0;
            unsigned int size = 0;
            unsigned int stride = 0;
            const void* pointer = nullptr;
        };
        void setVBO(VertexBuffer* vbo, const std::vector<VBO_DESC>& descVec, bool destoryHold = true);

        void clear(bool destoryHold = true);

        
        void setDrawMode(RenderProtocol::DrawMode mode)             { mDrawMode = mode; }
        void setFirst(unsigned int first)                           { mFirst = first; }
        void setCount(unsigned int count)                           { mCount = count; }
        void setElementType(RenderProtocol::DrawElementType type)   { mElementType = type; }
        void setIndices(const void* indices)                        { mIndices = indices; }
        
        RenderProtocol::DrawMode        getDrawMode()       const   { return mDrawMode; }
        unsigned int                    getFirst()          const   { return mFirst; }
        unsigned int                    getCount()          const   { return mCount; }
        RenderProtocol::DrawElementType getElementType()    const   { return mElementType; }
        const void*                     getIndices()        const   { return mIndices; }

        enum class RenderType
        {
            Array,
            Element
        };
        void        setRenderType(RenderType type)                  { mRenderType = type; }
        RenderType  getRenderType()                         const   { return mRenderType; }

    private:
        VertexArrayObject*  mVAO = nullptr;  
        VertexBuffer*       mVBO = nullptr;  
        IndexBuffer*        mIBO = nullptr; 

        RenderProtocol::DrawMode mDrawMode = RenderProtocol::DrawMode::Triangles;
        unsigned int mFirst = 0;
        unsigned int mCount = 0;

        RenderProtocol::DrawElementType mElementType = RenderProtocol::DrawElementType::UnsignedInt;
        const void* mIndices = nullptr;

        RenderType mRenderType = RenderType::Element;
    };
}