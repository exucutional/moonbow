#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#include <GLFW/glfw3native.h>

namespace moonbow
{

struct Window
{
    GLFWwindow* window;
    int width;
    int height;
};

Window init_window(const char* window_title, int width, int height);

} // namespace moonbow
