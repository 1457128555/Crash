#include "CrashScene.h"

class HelloInsDraw07 : public Crash::Scene
{
public:
    HelloInsDraw07(): Scene("HelloInsDraw07"){};

    virtual void initialize()               override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};