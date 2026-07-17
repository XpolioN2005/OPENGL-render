#include "shader/ShaderParser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string ShaderParser::ReadFile(const std::string &filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }

    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

ShaderParser::StageMap ShaderParser::Parse(const std::string &filepath) {
    StageMap stages;

    std::string source = ReadFile(filepath);

    std::istringstream stream(source);

    std::string globalSource;
    std::string line;

    ShaderStage currentStage = ShaderStage::None;

    while (std::getline(stream, line)) {
        // Ignore blank lines before any stage starts.
        if (currentStage == ShaderStage::None && line.find_first_not_of(" \t\r") == std::string::npos) {
            continue;
        }

        if (line == "#[end]") {

            if (currentStage == ShaderStage::None) {
                throw std::runtime_error("Unexpected #[end] directive.");
            }

            currentStage = ShaderStage::None;
            continue;
        }

        ShaderStage stage = ParseDirective(line);

        if (stage != ShaderStage::None) {

            if (currentStage != ShaderStage::None) {
                throw std::runtime_error("Cannot start a new shader stage before ending the current one.");
            }

            currentStage = stage;
            stages[currentStage] = globalSource;
            continue;
        }

        // Normal GLSL code.
        if (currentStage == ShaderStage::None) {
            globalSource += line + '\n';
        } else {
            stages[currentStage] += line + '\n';
        }
    }

    if (currentStage != ShaderStage::None) {
        throw std::runtime_error("Shader stage was not terminated with #[end].");
    }

    if (stages.find(ShaderStage::Vertex) == stages.end()) {
        throw std::runtime_error("Vertex shader stage is missing.");
    }

    if (stages.find(ShaderStage::Fragment) == stages.end()) {
        throw std::runtime_error("Fragment shader stage is missing.");
    }

    return stages;
}

ShaderStage ShaderParser::ParseDirective(const std::string &line) {
    if (line == "#[vertex]")
        return ShaderStage::Vertex;
    if (line == "#[fragment]")
        return ShaderStage::Fragment;
    if (line == "#[geometry]")
        return ShaderStage::Geometry;
    if (line == "#[compute]")
        return ShaderStage::Compute;

    return ShaderStage::None;
}