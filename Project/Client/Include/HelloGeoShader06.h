#include "CrashScene.h"

class HelloGeoShader06 : public Crash::Scene
{
public:
    HelloGeoShader06(): Scene("HelloGeoShader06"){};

    virtual void initialize()               override;
    virtual void renderScene()              override;
    virtual void shutdown()                 override;
};