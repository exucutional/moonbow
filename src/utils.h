// https://github.com/PacktPublishing/3D-Graphics-Rendering-Cookbook-Second-Edition/blob/main/shared/Utils.h

#pragma once

#include <memory>
#if !defined(__APPLE__)
#include <malloc.h>
#endif

#include <glslang/Include/glslang_c_interface.h>
#include <ldrutils/lutils/ScopeExit.h>
#include <lvk/LVK.h>
#include <lvk/vulkan/VulkanUtils.h>

lvk::Holder<lvk::ShaderModuleHandle> load_shader_module(const std::unique_ptr<lvk::IContext>& ctx, const char* fileName);
