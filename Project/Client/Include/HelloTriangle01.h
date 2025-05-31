#include "CrashScene.h"

class HelloTriangle01 : public Crash::Scene
{
public:
    HelloTriangle01();
    virtual ~HelloTriangle01()              override{};

    virtual void initialize()               override;
    virtual void update(float deltaTime)    override{};
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
private:


};