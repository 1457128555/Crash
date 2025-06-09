#include "CrashScene.h"

class HelloSkyCube05 : public Crash::Scene
{
public:
    HelloSkyCube05(): Scene("HelloSkyCube05"){};

    virtual void initialize()               override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};