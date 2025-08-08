#include "glfw_window.h"
#include "GLFW/glfw3.h"

#include <cassert>

namespace moonbow
{

Window init_window(const char *window_title, int width, int height)
{
    glfwSetErrorCallback([](int error, const char* description) {
        printf("GLFW Error (%d): %s\n", error, description);
    });

    const bool isInited = glfwInit();
    assert(isInited);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    assert(monitor);
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    assert(mode);
    

    GLFWwindow *glfwWindow = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (!glfwWindow)
    {
        glfwTerminate();
        return {};
    }

    glfwSetKeyCallback(glfwWindow, [](GLFWwindow *window, int key, int, int action, int) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });

    Window window { .window = glfwWindow, };
    glfwGetWindowSize(glfwWindow, &window.width, &window.height);
    return window;
}

} // namespace moonbow
