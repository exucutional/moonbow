#include "GLFW/glfw3.h"
#include "glfw_window.h"

static constexpr uint32_t WIDTH = 1680;
static constexpr uint32_t HEIGHT = 720;

class WindowApp final
{
public:
    WindowApp(const char *name, int width, int height)
    {
       windowInfo = moonbow::init_window(name, width, height);
    }

    void run()
    {
        while (!glfwWindowShouldClose(windowInfo.window))
            glfwPollEvents();
    }

    ~WindowApp()
    {
        glfwDestroyWindow(windowInfo.window);
        glfwTerminate();
    }
private:
    moonbow::Window windowInfo;
};

int main()
{
    WindowApp app("Moonbow", WIDTH, HEIGHT);
    app.run();
    return 0;
}