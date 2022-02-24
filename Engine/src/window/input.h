#ifndef FLY_INPUT_H
#define FLY_INPUT_H

#include "../core/types.h"
#include "keys.h"

struct GLFWwindow;

namespace Fly {
    class Input {
        friend class Window;
    public:
        static bool GetKey(Key key);
        static bool GetKeyDown(Key key);
        static bool GetKeyUp(Key key);
        static bool GetMouseButton(MouseButton mouseButton);
        static bool GetMouseButtonDown(MouseButton mouseButton);
        static bool GetMouseButtonUp(MouseButton mouseButton);
        static void GetMousePosition(f32* x, f32* y);
    private:
        static void Update(GLFWwindow* windowPtr); // called by window
        
        static bool s_KeyboardState[512];
        static bool s_KeyboardStatePrevious[512];
        static bool s_MouseState[8];
        static bool s_MouseStatePrevious[8];
        static f32 s_MousePositionX;
        static f32 s_MousePositionY;
    };
}

#endif /* End of FLY_INPUT_H */
