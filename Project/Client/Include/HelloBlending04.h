#include "CrashScene.h"

class HelloBlending04 : public Crash::Scene
{
public:
    HelloBlending04();

    virtual void initialize()               override;
    virtual void update(float deltaTime)    override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};