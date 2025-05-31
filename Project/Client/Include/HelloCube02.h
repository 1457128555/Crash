#include "CrashScene.h"

class HelloCube02 : public Crash::Scene
{
public:
    HelloCube02();
    virtual ~HelloCube02()                  override{};

    virtual void initialize()               override;
    virtual void update(float deltaTime)    override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};