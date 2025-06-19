#pragma once
#include "CrashSingleton.hpp"

class EditorRoot : public Crash::Singleton<EditorRoot>
{
public:
    explicit EditorRoot(void* wHnd, const std::string& glsl_version);
    virtual ~EditorRoot() override;

    void frameBegin();
    void frameEnd();
    void updatePlatformWindows();

    void render();

    bool checkViewportsEnable();
};