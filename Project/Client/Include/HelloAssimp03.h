#include "CrashScene.h"

class HelloAssimp03 : public Crash::Scene
{
public:
    HelloAssimp03();
    virtual ~HelloAssimp03()                override{};

    virtual void initialize()               override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};