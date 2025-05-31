#pragma once

#include "CrashEngineDefines.hpp"
#include "CrashRenderProtocol.hpp"

#include <string>
namespace Crash
{
    class CRASH_ENGINE_API FileSystem
    {
    public:
       static std::string ReadFile(const std::string& filePath);
       static std::string ReadShader(const std::string& filePath);

       static unsigned char* ReadImage(const std::string& filePath, int& width, int& height, RenderProtocol::TexFormat& fmt);
       static void FreeImage(unsigned char* data);
    
       static inline bool(*sAndroidRead)(const std::string& path, std::string& data) = nullptr;
    };
}