#include "timer.h"

namespace Fly {
    void Timer::Start() {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    f32 Timer::GetElapsedTimeS() const {
        std::chrono::duration<f32> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
        return duration.count();
    }

    f32 Timer::GetElapsedTimeMS() const {
        std::chrono::duration<f32, std::milli> duration =
            std::chrono::high_resolution_clock::now() - m_StartTime;
        return duration.count();
    }
}
