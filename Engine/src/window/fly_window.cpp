#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../core/fly_gl.h"
#include "../core/log.h"
#include "../renderer/renderer.h"
#include "../math/general.h"
#include "fly_window.h"
#include "input.h"

namespace Fly {
    Window* g_Window = nullptr;
    
    // Callbacks:
    void Window::OnKeyboardPress(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Input::s_KeyboardState[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            Input::s_KeyboardState[key] = false;
        }
    }

    void Window::OnMousePress(GLFWwindow* window, i32 button, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Input::s_MouseState[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            Input::s_MouseState[button] = false;
        }
    }

    void Window::OnWindowResize(GLFWwindow* window, i32 width, i32 height) {
        g_Renderer->OnWindowResize(width, height);
    }

    
    void Window::SetFullScreen(i32 width, i32 height) {
        // switch to full screen
        glfwSetWindowSize(m_WindowPtr, width, height);
        i32 xpos, ypos;
        glfwGetWindowPos(m_WindowPtr, &xpos, &ypos);
        g_Window->OnWindowResize(m_WindowPtr, width, height);
        glfwSetWindowMonitor(m_WindowPtr, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);
    }

    void Window::SetWindowedScreen(i32 width, i32 height) {
        i32 monitorX, monitorY;
        glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);

        glfwSetWindowSize(m_WindowPtr, width, height);
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(m_WindowPtr, NULL, monitorX + (mode->width - width)/2, monitorY + (mode->height - height)/2, width, height, GLFW_DONT_CARE);
        //glfwSetWindowPos(m_WindowPtr,
        //             monitorX + (mode->width - width) / 2,
        //             monitorY + (mode->height - height) / 2);
    }

    // Logic:
    i32 Window::Create(const char* name, i32 width, i32 height, bool fullScreen) {
        if (!glfwInit()) {
            FLY_WARN("Failed to init glfw");
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        
        
        // For OSX support
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWmonitor* monitor = NULL;
        if (fullScreen) {
            monitor = glfwGetPrimaryMonitor();
        }
        m_WindowPtr = glfwCreateWindow(width, height, name, monitor, NULL);
        glfwSetWindowSizeLimits(m_WindowPtr, 1024, 576, GLFW_DONT_CARE, GLFW_DONT_CARE);

        
        if (!m_WindowPtr) {
            FLY_WARN("Failed to create window");
            glfwTerminate();
            return -1;
        }

        // making gl current context
        glfwMakeContextCurrent(m_WindowPtr);
        glfwSwapInterval(1);

        glfwSetKeyCallback(m_WindowPtr, OnKeyboardPress);
        glfwSetMouseButtonCallback(m_WindowPtr, OnMousePress);
        glfwSetWindowSizeCallback(m_WindowPtr, OnWindowResize);

        glfwSetInputMode(m_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable cursor
        
        m_Inited = true;
        return 0;
    }

    void Window::ShowCursor(bool value) {
        if (value) {
            glfwSetInputMode(m_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // disable cursor
        }
        else {
            glfwSetInputMode(m_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable cursor
        }
    }

    i32 Window::GetWidth() const {
        i32 w, h;
        glfwGetFramebufferSize(m_WindowPtr, &w, &h);
        return w;
    }

    i32 Window::GetHeight() const {
        i32 w, h;
        glfwGetFramebufferSize(m_WindowPtr, &w, &h);
        return h;
    }
    
    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_WindowPtr);
    }

    void Window::Close() {
        glfwSetWindowShouldClose(m_WindowPtr, true);
    }

    void Window::PollEvents() {
        Input::Update(m_WindowPtr); // copy input of previous frame first
        glfwPollEvents(); // poll new input
    }
    
    void Window::SwapBuffers() {
        glfwSwapBuffers(m_WindowPtr);
    }

    Window::~Window() {
        if (m_Inited) {
            glfwDestroyWindow(m_WindowPtr);
            glfwTerminate();
        }
    }
}
