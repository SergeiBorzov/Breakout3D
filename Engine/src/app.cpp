#ifdef WIN32
#include <windows.h>
#endif

#include <filesystem>
#include <time.h>

#include "core/log.h"
#include "window/fly_window.h"
#include "renderer/renderer.h"
#include "physics/physics_world.h"
#include "script/script_manager.h"
#include "resource/resource_manager.h"
#include "gui/canvas.h"
#include "app.h"

namespace fs = std::filesystem;

namespace Fly {
    fs::path App::s_Path;

    i32 App::Init(const char* name, i32 width, i32 height, bool fullScreen) {
        srand((u32)time(0)); // set random seed

        if (StoreAppPath() != 0) {
            return -1;
        }
        
        g_Window = new Window();
        if (g_Window->Create(name, width, height, fullScreen) != 0) {
            return -1;
        }

        g_ResourceManager = new ResourceManager();
        g_Renderer = new Renderer();
        if (g_Renderer->Init() != 0) {
            return -1;
        }

        UI::g_Canvas = new UI::Canvas();
        g_PhysicsWorld = new PhysicsWorld();
        if (g_PhysicsWorld->Init() != 0) {
            return -1;
        }
        g_ScriptManager = new ScriptManager();

        m_Inited = true;
        return 0;
    }

    i32 App::StoreAppPath() {
#ifdef WIN32
        char appPath[MAX_PATH]; 
        HMODULE hModule = GetModuleHandle(NULL);
        if (hModule != NULL) {
            if (GetModuleFileNameA(hModule, appPath, (sizeof(appPath))) != 0) {
                s_Path = fs::path(appPath).parent_path();
            }
            else {
                return -1;
            } 
        }
        return 0;
#else
    FLY_WARN("This platform is not supported!");
    return -1;
#endif
    }
    
    void App::Run() {
        g_ScriptManager->Start();
        Start();
        m_Timer.Start();
        f32 previousTime = m_Timer.GetElapsedTimeS();
        f32 fixedDeltaTime = 0.0f;
        while (!g_Window->ShouldClose()) {
            f32 currentTime = m_Timer.GetElapsedTimeS();
            f32 deltaTime = currentTime - previousTime;
            fixedDeltaTime += deltaTime*m_TimeMultiplier;            
            previousTime = currentTime;
            g_Window->PollEvents();
            
            Update();
            g_ScriptManager->Update(deltaTime*m_TimeMultiplier);
            if (fixedDeltaTime >= m_FixedTimeStep) {
                f32 difference = fixedDeltaTime - m_FixedTimeStep;
                deltaTime -= difference;
                g_ScriptManager->FixedUpdate(m_FixedTimeStep);
                g_PhysicsWorld->RunSimulation(m_FixedTimeStep);
                fixedDeltaTime = 0.0f;
            }

            g_ScriptManager->ReleaseMemory();
            g_ScriptManager->Start();

            UI::g_Canvas->Update(deltaTime); // UI is not affected by timeMultiplier!
            g_Renderer->Draw();
            g_Window->SwapBuffers();
        }
    }

    App::~App() {
        if (m_Inited) {
            delete g_ScriptManager;
            delete g_PhysicsWorld;
            delete UI::g_Canvas;
            delete g_Renderer;
            delete g_ResourceManager;
            delete g_Window;
        }
    }
}
