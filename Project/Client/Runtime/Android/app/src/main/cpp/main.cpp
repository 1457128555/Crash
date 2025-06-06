#include <jni.h>

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>

#include "CrashPCH.h"
#include "CrashEngine.h"
#include "CrashInputMgr.h"
#include "CrashFileSystem.h"
#include "CrashLogManager.h"
#include "HelloAssimp03.h"

using namespace Crash;

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

struct TouchStick 
{
    float   x           = 0;            // [-1,1]
    float   y           = 0;            // [-1,1]
    bool    active      = false;
    int     pointerId   = -1;
};
TouchStick gMoveStick, gViewStick;

EGLDisplay g_display = EGL_NO_DISPLAY;
EGLSurface g_surface = EGL_NO_SURFACE;
EGLContext g_context = EGL_NO_CONTEXT;
android_app*   g_app = nullptr;
Crash::Scene*  g_Scene= nullptr;

void handleInput()
{
    auto* inputBuffer = android_app_swap_input_buffers(g_app);
    if (!inputBuffer) 
        return;

    int screenWidth = ANativeWindow_getWidth(g_app->window);
    int screenHeight = ANativeWindow_getHeight(g_app->window);


    int controlLeftRight = screenWidth;
    int controlTopBottom = screenHeight;

    for (auto i = 0; i < inputBuffer->motionEventsCount; ++i) 
    {
        auto &motionEvent = inputBuffer->motionEvents[i];
        int action = motionEvent.action & AMOTION_EVENT_ACTION_MASK;
        int pointerCount = motionEvent.pointerCount;

        for (int j = 0; j < pointerCount; ++j) 
        {
            float x = motionEvent.pointers[j].axisValues[AMOTION_EVENT_AXIS_X];
            float y = motionEvent.pointers[j].axisValues[AMOTION_EVENT_AXIS_Y];
            int id = motionEvent.pointers[j].id;

            int curLeftRight = x;
            int curTopBottom = y;

            bool isLeft = (curLeftRight < controlLeftRight / 2.f);
            TouchStick& stick =  isLeft ? gMoveStick : gViewStick;

            if (action == AMOTION_EVENT_ACTION_DOWN || action == AMOTION_EVENT_ACTION_POINTER_DOWN)
            {
                stick.active = true;
                stick.pointerId = id;
                stick.x = curLeftRight;
                stick.y = curTopBottom;
            }
            else if (action == AMOTION_EVENT_ACTION_MOVE && stick.active && stick.pointerId == id)
            {
                float offsetLeftRight = curLeftRight - stick.x;
                float offsetTopBottom = curTopBottom - stick.y;
                if(isLeft)
                {
                    offsetTopBottom = -offsetTopBottom; // Invert Y-axis for left stick
                    
                    if (offsetLeftRight < -10.f)
                        Crash::InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_A);
                    if (offsetLeftRight >  10.f)
                        Crash::InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_D);
                    if (offsetTopBottom < -10.f)
                        Crash::InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_S);
                    if (offsetTopBottom >  10.f)
                        Crash::InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_W);
                }
                else
                {
                    Crash::InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::MOUSE_BUTTON_RIGHT);
                    Crash::InputMgr::Instance()->setMouseDelta(offsetLeftRight / 10.f, offsetTopBottom / 10.f);
                }

                LogManager::Instance()->log(
                    std::string("Action:")+
                    "TouchStick " + std::string(isLeft ? "Move" : "View") +
                    ", beg_x=" + std::to_string(stick.x) +
                    ", beg_y=" + std::to_string(stick.y) +
                    ", cur_x =" + std::to_string(curLeftRight) +
                    ", cur_y =" + std::to_string(curTopBottom) +
                    ", off_x=" + std::to_string(offsetLeftRight) +
                    ", off_y=" + std::to_string(offsetTopBottom)

                );
            }
            else if ((action == AMOTION_EVENT_ACTION_UP || action == AMOTION_EVENT_ACTION_POINTER_UP)
                        && stick.active && stick.pointerId == id)
            {
                stick.active = false;
                stick.pointerId = -1;
                stick.x = 0;
                stick.y = 0;
            }
        }
    }

    android_app_clear_motion_events(inputBuffer);
}

std::string g_filesDir;
std::string g_externalDir;
std::string g_packageName;

JNIEXPORT void JNICALL
Java_com_example_runtime_MainActivity_nativeSetAppPaths(JNIEnv* env, jclass,
                                                       jstring filesDir,
                                                       jstring externalDir,
                                                       jstring packageName) {
    const char* c_filesDir = env->GetStringUTFChars(filesDir, nullptr);
    const char* c_externalDir = env->GetStringUTFChars(externalDir, nullptr);
    const char* c_packageName = env->GetStringUTFChars(packageName, nullptr);

    g_filesDir = c_filesDir ? c_filesDir : "";
    g_externalDir = c_externalDir ? c_externalDir : "";
    g_packageName = c_packageName ? c_packageName : "";

    env->ReleaseStringUTFChars(filesDir, c_filesDir);
    env->ReleaseStringUTFChars(externalDir, c_externalDir);
    env->ReleaseStringUTFChars(packageName, c_packageName);

    FileSystem::sExternalDir = g_externalDir;
}

void handle_cmd(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
        {
            g_app = pApp;
            Crash::EngineConfig config = {};
            config.windowWidth  = ANativeWindow_getWidth(pApp->window);
            config.windowHeight = ANativeWindow_getHeight(pApp->window);
            config.outputFunc   = [](const std::string& log) {
                __android_log_print(ANDROID_LOG_DEBUG, "CrashRuntime", "%s", log.c_str());
            };
            config.procAddress  = reinterpret_cast<void*>(eglGetProcAddress);
            config.asyncRender      = true;
            config.initRenderContext = []() {
                // 1. 获取 EGLDisplay
                g_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                eglInitialize(g_display, 0, 0);

                // 2. 选择 EGLConfig
                const EGLint attribs[] = {
                        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES3_BIT,
                        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
                        EGL_BLUE_SIZE,          8,
                        EGL_GREEN_SIZE,         8,
                        EGL_RED_SIZE,           8,
                        EGL_ALPHA_SIZE,         8,
                        EGL_DEPTH_SIZE,         24,
                        EGL_STENCIL_SIZE,       8,
                        EGL_NONE
                };

                EGLConfig config;
                EGLint numConfigs;
                eglChooseConfig(g_display, attribs, &config, 1, &numConfigs);

                // 查询实际深度缓冲位数
                EGLint depthSize = 0;
                eglGetConfigAttrib(g_display, config, EGL_DEPTH_SIZE, &depthSize);
                __android_log_print(ANDROID_LOG_DEBUG, "CrashRuntime", "Actual EGL_DEPTH_SIZE: %d", depthSize);

                // 3. 创建 EGLSurface
                g_surface = eglCreateWindowSurface(g_display, config, g_app->window, nullptr);

                // 4. 创建 EGLContext
                const EGLint ctxAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
                g_context = eglCreateContext(g_display, config, EGL_NO_CONTEXT, ctxAttribs);

                // 5. 绑定上下文
                eglMakeCurrent(g_display, g_surface, g_surface, g_context);
            };

            config.swapBuffer = []() { auto swapResult = eglSwapBuffers(g_display, g_surface); };
            config.destroyRenderContext = [](){
                eglMakeCurrent(g_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                eglDestroySurface(g_display, g_surface);    g_surface = EGL_NO_SURFACE;
                eglDestroyContext(g_display, g_context);        g_context = EGL_NO_CONTEXT;
                eglTerminate(g_display);                            g_display = EGL_NO_DISPLAY;
            };

            config.assetPath = g_externalDir + "/assets/";
            new Crash::Engine(config);     
            g_Scene = new HelloAssimp03();
            Crash::Engine::Instance()->setScene(g_Scene);

        }break;
        case APP_CMD_TERM_WINDOW:
        {
            Crash::Engine::Instance()->setScene(nullptr);
            delete g_Scene;
            g_Scene = nullptr;
            delete Crash::Engine::Instance();
            g_app = nullptr;
        }break;
        default:
            break;
    }
}

bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

void android_main(struct android_app *pApp)
{
    pApp->onAppCmd = handle_cmd;
    //pApp->onInputEvent = handle_input;

    android_app_set_motion_event_filter(pApp, motion_event_filter_func);

    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    double currentTime = std::chrono::duration<double>(now - start).count();
    double lastTime = currentTime;

    while (!pApp->destroyRequested)
    {
        bool handleEvent = true;
        while (handleEvent)
        {
            int timeout = 0;
            int events;
            android_poll_source *pSource;
            int result = ALooper_pollOnce(timeout, nullptr, &events, reinterpret_cast<void**>(&pSource));

            switch (result) 
            {
                case ALOOPER_POLL_TIMEOUT:
                    [[clang::fallthrough]];
                case ALOOPER_POLL_WAKE:
                    // No events occurred before the timeout or explicit wake. Stop checking for events.
                    handleEvent = false;
                    break;
                case ALOOPER_EVENT_ERROR:
                    __android_log_print(ANDROID_LOG_DEBUG, "CrashRuntime", "%s", "ALooper_pollOnce returned an error");
                    break;
                case ALOOPER_POLL_CALLBACK:
                    break;
                default:
                    if (pSource) {
                        pSource->process(pApp, pSource);
                    }
            }
        }

        if(g_app)
        {
            handleInput();

            now = std::chrono::steady_clock::now();
            currentTime = std::chrono::duration<double>(now - start).count();
            double deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            Crash::Engine::Instance()->setExecuteTime(currentTime);
            Crash::Engine::Instance()->update(deltaTime);

            Crash::Engine::Instance()->renderFrame();

            Crash::Engine::Instance()->renderFinish();
        }
    }
}
}