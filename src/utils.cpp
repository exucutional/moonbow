// https://github.com/PacktPublishing/3D-Graphics-Rendering-Cookbook-Second-Edition/blob/main/shared/Utils.cpp

#include <string.h>
#include <string>
#if !defined(__APPLE__)
#include <malloc.h>
#endif

#include "utils.h"

#include <stb/stb_image.h>
#include <ktx.h>
#include <ktx-software/lib/gl_format.h>

bool ends_with(const char* s, const char* part)
{
  const size_t sLength    = strlen(s);
  const size_t partLength = strlen(part);
  return sLength < partLength ? false : strcmp(s + sLength - partLength, part) == 0;
}

std::string read_shader_file(const char* fileName)
{
  FILE* file = fopen(fileName, "r");

  if (!file) {
    LLOGW("I/O error. Cannot open shader file '%s'\n", fileName);
    return std::string();
  }

  fseek(file, 0L, SEEK_END);
  const size_t bytesinfile = ftell(file);
  fseek(file, 0L, SEEK_SET);

  char* buffer           = (char*)alloca(bytesinfile + 1);
  const size_t bytesread = fread(buffer, 1, bytesinfile, file);
  fclose(file);

  buffer[bytesread] = 0;

  static constexpr unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };

  if (bytesread > 3) {
    if (!memcmp(buffer, BOM, 3))
      memset(buffer, ' ', 3);
  }

  std::string code(buffer);

  while (code.find("#include ") != code.npos) {
    const auto pos = code.find("#include ");
    const auto p1  = code.find('<', pos);
    const auto p2  = code.find('>', pos);
    if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
      LLOGW("Error while loading shader program: %s\n", code.c_str());
      return std::string();
    }
    const std::string name    = code.substr(p1 + 1, p2 - p1 - 1);
    const std::string include = read_shader_file(name.c_str());
    code.replace(pos, p2 - pos + 1, include.c_str());
  }

  return code;
}

lvk::ShaderStage lvk_shader_stage_from_file_name(const char* fileName)
{
  if (ends_with(fileName, ".vert"))
    return lvk::Stage_Vert;

  if (ends_with(fileName, ".frag"))
    return lvk::Stage_Frag;

  if (ends_with(fileName, ".geom"))
    return lvk::Stage_Geom;

  if (ends_with(fileName, ".comp"))
    return lvk::Stage_Comp;

  if (ends_with(fileName, ".tesc"))
    return lvk::Stage_Tesc;

  if (ends_with(fileName, ".tese"))
    return lvk::Stage_Tese;

  return lvk::Stage_Vert;
}

lvk::Holder<lvk::ShaderModuleHandle> load_shader_module(const std::unique_ptr<lvk::IContext>& ctx, const char* fileName) {
  const std::string code = read_shader_file(fileName);
  const lvk::ShaderStage stage = lvk_shader_stage_from_file_name(fileName);

  if (code.empty()) {
    return {};
  }

  lvk::Result res;

  lvk::Holder<lvk::ShaderModuleHandle> handle =
      ctx->createShaderModule({ code.c_str(), stage, (std::string("Shader Module: ") + fileName).c_str() }, &res);

  if (!res.isOk()) {
    return {};
  }

  return handle;
}

