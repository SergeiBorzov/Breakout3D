#ifndef FLY_WINDOW_H
#define FLY_WINDOW_H

#include "../core/platform.h"
#include "../core/types.h"

struct GLFWwindow;

namespace Fly {    
    class Window {
    public:
        i32 Create(const char* name, i32 width, i32 heigth, bool fullScreen);
        void PollEvents();
        void SwapBuffers();

        bool ShouldClose() const;
        void Close();
        i32 GetWidth() const;
        i32 GetHeight() const;

        void SetFullScreen(i32 width, i32 height);
        void SetWindowedScreen(i32 width, i32 height);

        void ShowCursor(bool value);
        ~Window();
    private:
        static void OnKeyboardPress(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
        static void OnMousePress(GLFWwindow* window, i32 button, i32 action, i32 mods);
        static void OnMouseMove(GLFWwindow* window, f64 x, f64 y);
        static void OnWindowResize(GLFWwindow* window, i32 width, i32 height);
        GLFWwindow* m_WindowPtr = NULL;
        bool m_Inited = false;
    };
    

   FLY_API extern Window* g_Window;  

}


#endif /* End of FLY_WINDOW_H */
