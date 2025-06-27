@echo off
echo Compiling shaders...

%VULKAN_SDK%/Bin/glslc shader.vert -o vert.spv
if %errorlevel% neq 0 (
    echo Error compiling vertex shader!
    pause
    exit /b 1
)

%VULKAN_SDK%/Bin/glslc shader.frag -o frag.spv
if %errorlevel% neq 0 (
    echo Error compiling fragment shader!
    pause
    exit /b 1
)

echo Shader compilation completed successfully!
pause