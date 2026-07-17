#pragma once

#include <string>
#include <unordered_map>

enum class ShaderStage {
    None,
    Vertex,
    Fragment,
    Geometry,
    Compute
};

class ShaderParser {
  public:
    using StageMap = std::unordered_map<ShaderStage, std::string>;

    StageMap Parse(const std::string &filepath);

  private:
    std::string ReadFile(const std::string &filepath);

    ShaderStage ParseDirective(const std::string &line);
};