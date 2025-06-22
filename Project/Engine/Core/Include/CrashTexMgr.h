#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashSingleton.hpp"
#include "CrashRenderProtocol.hpp"

#include <string>

namespace Crash
{
    class Texture;
    class CRASH_ENGINE_API TexMgr : public Singleton<TexMgr>
    {
    public:
        TexMgr()  = default;
        ~TexMgr() = default;

        std::shared_ptr<Texture> createCubeMapTexture(const std::string& name, const std::array<std::string, 6>& faces);
        std::shared_ptr<Texture> createTexture(const std::string& name, RenderProtocol::TexType type);
        void destroyTexture(Texture* texture);

    private:
        std::unordered_map<std::string, std::weak_ptr<Texture>> mTexCache;
    };
} // namespace Crash