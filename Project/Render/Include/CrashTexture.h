#pragma once

#include "CrashRenderProtocol.hpp"

#include <string>
namespace Crash
{
    class Texture
    {
        friend class RenderSystem;
    protected:
        Texture(const std::string& name, RenderProtocol::TexType type);
        virtual ~Texture();

        void                        createHandle();     
        void                        bind()                  const;
        void                        unbind()                const;
        unsigned int                getID()                 const { return mID; }
        RenderProtocol::TexType     getType()               const { return mType; }
        const std::string&          getName()               const { return mName; }
        int                         getWidth()              const { return mWidth; }
        int                         getHeight()             const { return mHeight; }
        RenderProtocol::TexFormat   getFormat()             const { return mFormat; }
        bool                        getGenerateMipmap()     const { return mGenerateMipmap; }

        void setTextureData(int level, RenderProtocol::TexFormat internalFormat, int width, int height, 
            RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data, bool generateMipmap);

        void setTextureWarpMode(RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround);
        void setTextureFilterMode(RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType);

    protected:
        unsigned int                mID     =   0u;
        const std::string           mName   =   "Texture2D";
        RenderProtocol::TexType     mType   =   RenderProtocol::TexType::Texture2D;

        int                         mWidth  =   0;
        int                         mHeight =   0;
        RenderProtocol::TexFormat   mFormat = RenderProtocol::TexFormat::RGBA;
        bool                        mGenerateMipmap = true;
    };
}




//     class CRASH_ENGINE_API Texture2D : public Texture
//     {
//     public:
//         explicit Texture2D(const std::string& texName);
        
//     private:
//         const std::string mTexName = "Texture2D";
//         int               mWidth     = 0;
//         int               mHeight    = 0;
//     };
// }
