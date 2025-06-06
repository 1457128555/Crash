#include "CrashPCH.h"
#include "CrashFileSystem.h"
#include "CrashLogManager.h"
#include "CrashEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "CrashStbImage.h"

namespace Crash
{
    namespace
    {
        RenderProtocol::TexFormat ChannelsToTexFormat(int channels)
        {
            switch (channels)
            {
                case 1:     return RenderProtocol::TexFormat::Red;
                case 2:     return RenderProtocol::TexFormat::RG;
                case 3:     return RenderProtocol::TexFormat::RGB;
                case 4:     return RenderProtocol::TexFormat::RGBA;
                default:    break; 
            }
            assert(false && "Invalid number of channels for texture format");
            return RenderProtocol::TexFormat::RGBA; 
        }
    }

    std::string FileSystem::ReadFile(const std::string& filePath)
    {
        std::string fullPath = Engine::Instance()->getAssetPath() + filePath;

        std::string data;
        std::ifstream file(fullPath, std::ios::binary);
        if (!file.is_open())
            LogManager::Instance()->log("Failed to open file: " + fullPath);

        data = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return data;
    }

    std::string FileSystem::ReadShader(const std::string& filePath)
    {
        static const std::string shaderPath = R"(Shader/)";
        static const std::regex includeRegex(R"(#include\s+\"([^\"]+)\")");

        std::string result; //  new shader code

        std::string _src = ReadFile(shaderPath + filePath);
        std::istringstream file(_src);
        std::string line;

        while (std::getline(file, line))
        {
            {
                std::string trimmed = line;

                trimmed.erase(0, trimmed.find_first_not_of(" \t"));
                if (trimmed.rfind("//", 0) == 0)
                {
                    result += line + "\n"; 
                    continue;
                }
            }

            std::smatch match;
            if (std::regex_search(line, match, includeRegex))
            {
                if (match.size() > 1)
                {
                    std::string includeFile = match[1].str();
                    std::string includeSrc = ReadShader(includeFile);
                    if (!includeSrc.empty())
                    {
                        result += includeSrc; 
                    }
                    else
                    {
                        LogManager::Instance()->log("Failed to read included shader: " + includeFile);
                    }
                }
            }
            else
            {
                result += line + "\n"; 
            }
        }

        return result;
    }
    
    unsigned char* FileSystem::ReadImage(const std::string& filePath, int& width, int& height, RenderProtocol::TexFormat& fmt)
    {

        const std::string textureHead = R"(Texture/)";
        const std::string texPath = textureHead + filePath;
        const std::string imageData = ReadFile(texPath);
        
        int channels = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load_from_memory((const stbi_uc*)imageData.data(), (int)imageData.size(), &width, &height, &channels, 0);

        if(data)
            fmt = ChannelsToTexFormat(channels);
        return data;
    }

    void FileSystem::FreeImage(unsigned char* data)
    {
        if (data)
        {
            stbi_image_free(data);
        }
    }
} // namespace Crash
