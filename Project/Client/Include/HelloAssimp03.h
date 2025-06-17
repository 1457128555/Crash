#include "CrashScene.h"

class HelloAssimp03 : public Crash::Scene
{
public:
    HelloAssimp03() : Scene("HelloAssimp03"){};

    virtual void initialize()               override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};