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

        Texture* createTexture(const std::string& name, RenderProtocol::TexType type);
        void destroyTexture(Texture* texture);
    private:

    };
} // namespace Crash