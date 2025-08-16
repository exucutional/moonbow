#include <lvk/LVK.h>
#include <GLFW/glfw3.h>

#include "utils.h"

static constexpr uint32_t WIDTH = 1680;
static constexpr uint32_t HEIGHT = 720;

static const char* LOG_FILE_PATH = ".log.last.txt";

static const char* VERT_PATH = "src/shaders/triangle.vert";
static const char* FRAG_PATH = "src/shaders/triangle.frag";

class WindowApp final
{
public:
    WindowApp(const char *name, int width, int height)
    : window(lvk::initWindow(name, width, height), glfwDestroyWindow)
    {
        ctx = lvk::createVulkanContextWithSwapchain(window.get(), width, height, {});
        initRender();
    }
    
    void initRender()
    {
        vert = load_shader_module(ctx, VERT_PATH);
        frag = load_shader_module(ctx, FRAG_PATH);
        pipeline = ctx->createRenderPipeline({
            .smVert = vert,
            .smFrag = frag,
            .color  = { { .format = ctx->getSwapchainFormat() } },
        });
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

            buf.cmdBeginRendering(
                { .color = { { .loadOp = lvk::LoadOp_Clear, .clearColor = { 1.0f, 1.0f, 1.0f, 1.0f } } } },
                { .color = { { .texture = ctx->getCurrentSwapchainTexture() } } }
            );

            buf.cmdBindRenderPipeline(pipeline);
            buf.cmdPushDebugGroupLabel("Render Triangle", 0xff0000ff);
            buf.cmdDraw(3);
            buf.cmdPopDebugGroupLabel();

            buf.cmdEndRendering();

            ctx->submit(buf, ctx->getCurrentSwapchainTexture());
        }
    }

    ~WindowApp()
    {
        window.reset();
        vert.reset();
        frag.reset();
        pipeline.reset();
        ctx.reset();
        glfwTerminate();
    }
private:
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;
    std::unique_ptr<lvk::IContext> ctx;

    lvk::Holder<lvk::ShaderModuleHandle> vert;
    lvk::Holder<lvk::ShaderModuleHandle> frag;
    lvk::Holder<lvk::RenderPipelineHandle> pipeline;
};

int main()
{
    minilog::initialize(LOG_FILE_PATH, { .threadNames = false });
    WindowApp app("Moonbow", WIDTH, HEIGHT);
    app.run();
    return 0;
}
