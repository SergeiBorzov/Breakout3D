#ifndef FLY_APP_H
#define FLY_APP_H

#include <filesystem>

#include "core/platform.h"
#include "core/types.h"
#include "core/timer.h"

namespace Fly {
    class App {
    public:
        virtual void Start() {}
        virtual void Update() {}

        i32 Init(const char* name, i32 width, i32 height, bool fullScreen);
        void Run();

        static std::filesystem::path GetPath() { return s_Path; }
        
        ~App();
    protected:
        f32 m_TimeMultiplier = 1.0f;
        const f32 m_FixedTimeStep = 0.005f;
    private:
        FLY_API static std::filesystem::path s_Path;
        i32 StoreAppPath();

        Fly::Timer m_Timer;
        bool m_Inited = false;
    };
}

#endif /* End of FLY_APP_H */
