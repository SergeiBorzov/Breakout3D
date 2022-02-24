#include <string.h> // for memcpy

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.h"

namespace Fly {
    bool Input::s_KeyboardState[512] = { false };
    bool Input::s_KeyboardStatePrevious[512] = { false };
    bool Input::s_MouseState[8] = { false };
    bool Input::s_MouseStatePrevious[8] = { false };
    f32 Input::s_MousePositionX;
    f32 Input::s_MousePositionY;
    
    bool Input::GetKey(Key key) {
        return s_KeyboardState[(i32)key];
    }

    bool Input::GetKeyDown(Key key) {
        return s_KeyboardState[(i32)key] && !s_KeyboardStatePrevious[(i32)key];
    }

    bool Input::GetKeyUp(Key key) {
        return !s_KeyboardState[(i32)key] && s_KeyboardStatePrevious[(i32)key];
    }

    bool Input::GetMouseButton(MouseButton mouseButton) {
        return s_MouseState[(i32)mouseButton];
    }

    bool Input::GetMouseButtonDown(MouseButton mouseButton) {
        return s_MouseState[(i32)mouseButton] && !s_MouseStatePrevious[(i32)mouseButton];
    }

    bool Input::GetMouseButtonUp(MouseButton mouseButton) {
        return !s_MouseState[(i32)mouseButton] && s_MouseStatePrevious[(i32)mouseButton];
    }

    void Input::GetMousePosition(f32* x, f32* y) {
        *x = s_MousePositionX;
        *y = s_MousePositionY;
    }
    

    void Input::Update(GLFWwindow* windowPtr) {
        memcpy(s_KeyboardStatePrevious, s_KeyboardState, sizeof(bool)*512);
        memcpy(s_MouseStatePrevious, s_MouseState, sizeof(bool)*8);
        double x, y;
        glfwGetCursorPos(windowPtr, &x, &y);
        s_MousePositionX = (f32)x;
        s_MousePositionY = (f32)y;
    }
}
