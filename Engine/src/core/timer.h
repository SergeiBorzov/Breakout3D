#ifndef FLY_TIMER_H
#define FLY_TIMER_H

#include <chrono>

#include "types.h"

namespace Fly {
    class Timer {
    public:
        void Start();
        f32 GetElapsedTimeS() const;
        f32 GetElapsedTimeMS() const;
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    };
}

#endif /* End of FLY_TIMER_H */
