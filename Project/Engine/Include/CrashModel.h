// #pragma once

// #include "CrashEngineDefines.hpp"
// #include "CrashRenderable.h"

// #include <vector>
// #include <string>

// namespace Crash
// {
//     class Mesh;
//     class Texture;
//     class ShaderProgram;

//     class CRASH_ENGINE_API Model
//     {
//     public:
//         explicit Model(const std::string& name = "Define Model");
//         ~Model();

//         void draw(ShaderProgram* shader) const;

//     private:
//         const std::string       mName = "Define Model";

//         std::vector<Mesh*>      mMeshes;          
//     };
// }