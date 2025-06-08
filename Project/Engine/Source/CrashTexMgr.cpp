#include "CrashPCH.h"
#include "CrashTexMgr.h"
#include "CrashTexture.h"
#include "CrashRenderSystem.h"
#include "CrashFileSystem.h"

namespace Crash
{

    std::shared_ptr<Texture> TexMgr::createCubeMapTexture(const std::string& name,
         const std::array<std::string, 6>& faces)
    {
        auto findIt = mTexCache.find(name);
        if (findIt != mTexCache.end())
            return findIt->second.lock();

        Texture* texture = RenderSystem::Instance()->createTexture(name, RenderProtocol::TexType::TextureCubeMap);
        RenderSystem::Instance()->setTextureWarpMode(texture, RenderProtocol::TexWrap::S, RenderProtocol::TexSurround::ClampToEdge);
        RenderSystem::Instance()->setTextureWarpMode(texture, RenderProtocol::TexWrap::T, RenderProtocol::TexSurround::ClampToEdge);
        RenderSystem::Instance()->setTextureWarpMode(texture, RenderProtocol::TexWrap::R, RenderProtocol::TexSurround::ClampToEdge);
        
        int width = 0;
        int height = 0;
        RenderProtocol::TexFormat fmt = RenderProtocol::TexFormat::RGBA;
        std::array<unsigned char*, 6> faceData;
        for (size_t i = 0; i < faces.size(); ++i)
        {
            faceData[i] = FileSystem::ReadImage(faces[i], width, height, fmt, false);
            if (!faceData[i])
            {
                assert(false && "Failed to load texture face");
                LogManager::Instance()->log("Failed to load texture face: " + faces[i]);
                return nullptr;
            }
        }

        std::array<const void*, 6> faceDataConst = {
            faceData[0], faceData[1], faceData[2],
            faceData[3], faceData[4], faceData[5]
        };
        RenderSystem::Instance()->setCubeMapTextureData(texture, fmt, width, height,
            fmt, RenderProtocol::TexDataType::UnsignedByte, faceDataConst, false);

        for(auto data : faceData)
            FileSystem::FreeImage(data);

        std::shared_ptr<Texture> texPtr(texture, [](Texture* tex) {
                    RenderSystem::Instance()->destroyTexture(tex);
                });
        mTexCache[name] = texPtr;
        return texPtr;
    }

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
