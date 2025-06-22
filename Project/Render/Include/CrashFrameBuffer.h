#pragma once

#include "CrashRenderProtocol.hpp"

namespace Crash
{
    class Texture;

    class FrameBuffer
    {
    public:
        FrameBuffer(const std::string& name, int width, int height, int multiSamples, bool useRBO);
        virtual ~FrameBuffer();

        void createHandle();

        bool checkComplete()const;

        void bind() const;
        void unbind() const;

        unsigned int getID() const { return mID; }

        const std::shared_ptr<Texture>& getColorAttachment()           const { return mColorAttachment; }
        const std::shared_ptr<Texture>& getDepthStencilAttachment()    const { 
            assert(!mUseRBO && "DepthStencilAttachment is not available when using RBO!");
            return mDepthStencilAttachment; }

        int getWidth()  {return mWidth;};
        int getHeight() {return mHeight;};

    private:
        const std::string mName = "FrameBuffer";

        unsigned int    mID     = 0u;
        const int       mWidth  = 0;
        const int       mHeight = 0;
       
        const bool      mUseRBO = true; 
        unsigned int    mRBO    = 0u; 
        unsigned int    mMultiSamples = 0;
        
        std::shared_ptr<Texture> mColorAttachment;
        std::shared_ptr<Texture> mDepthStencilAttachment;
    };
}