#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader {
  public:
	explicit Shader(const std::string &filepath);
	~Shader();

	void Use() const;

	void SetBool(const std::string &name, bool value);
	void SetInt(const std::string &name, int value);
	void SetFloat(const std::string &name, float value);
	void SetMat4(const std::string &name, const glm::mat4 &value);

	// GL overloads will come later.
	// void SetVec2(...);
	// void SetVec3(...);
	// void SetVec4(...);

	GLuint GetID() const;

  private:
	GLuint m_Program = 0;

	std::unordered_map<std::string, GLint> m_UniformCache;

  private:
	void Compile(const std::string &filepath);
	GLuint CompileStage(GLenum type, const std::string &source);
	GLint GetUniformLocation(const std::string &name);
};