#include "CrashPCH.h"
#include "CrashTexMgr.h"
#include "CrashTexture.h"
#include "CrashRenderSystem.h"
#include "CrashFileSystem.h"

namespace Crash
{
    std::shared_ptr<Texture> TexMgr::createTexture(const std::string& name, RenderProtocol::TexType type)
    {
        auto findIt = mTexCache.find(name);
        if (findIt != mTexCache.end())
            return findIt->second.lock();

        Texture* texture = RenderSystem::Instance()->createTexture(name, type);

        int width = 0;
        int height = 0;
        RenderProtocol::TexFormat fmt = RenderProtocol::TexFormat::RGBA;

        unsigned char* data = FileSystem::ReadImage(name, width, height, fmt);
        if (data)
        {
            RenderSystem::Instance()->setTextureData(texture, 0, fmt, width, height,
                fmt, RenderProtocol::TexDataType::UnsignedByte, data, true);
            FileSystem::FreeImage(data);
        }
        
        std::shared_ptr<Texture> texPtr(texture, [](Texture* tex) {
            RenderSystem::Instance()->destroyTexture(tex);
        });
        mTexCache[name] = texPtr;
        return texPtr;
    }

    void TexMgr::destroyTexture(Texture* texture)
    {
        RenderSystem::Instance()->destroyTexture(texture);
        mTexCache.erase(texture->getName());
    }
} // namespace Crash
