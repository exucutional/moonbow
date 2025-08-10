#include <lvk/LVK.h>
#include <GLFW/glfw3.h>

static constexpr uint32_t WIDTH = 1680;
static constexpr uint32_t HEIGHT = 720;

static const char* LOG_FILE_PATH = ".log.last.txt";

class WindowApp final
{
public:
    WindowApp(const char *name, int width, int height)
    : window(lvk::initWindow(name, width, height), glfwDestroyWindow)
    {
        ctx = lvk::createVulkanContextWithSwapchain(window.get(), width, height, {});
    }

    void run()
    {
        while (!glfwWindowShouldClose(window.get()))
        {
            glfwPollEvents();

            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window.get(), &width, &height);

            if (!width || !height)
                continue;

            lvk::ICommandBuffer& buf = ctx->acquireCommandBuffer();
            ctx->submit(buf, ctx->getCurrentSwapchainTexture());
        }
    }

    ~WindowApp()
    {
        window.reset();
        ctx.reset();
        glfwTerminate();
    }
private:
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;
    std::unique_ptr<lvk::IContext> ctx;
};

int main()
{
    minilog::initialize(LOG_FILE_PATH, { .threadNames = false });
    WindowApp app("Moonbow", WIDTH, HEIGHT);
    app.run();
    return 0;
}
