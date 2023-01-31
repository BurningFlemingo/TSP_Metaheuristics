#pragma once

#include "pch.h"

std::string parseShader(const std::string& filePath);
uint compileShader(const std::string& shaderSource, uint shaderType);
uint createProgram(std::vector<uint> shaders);
uint createShaders(const std::vector<std::string>& shaderLocations, const std::vector<GLuint> shaderTypes);

int getUniformLocation(uint programID, const std::string& name);

void setUniform1i(uint programID, const std::string& name, int value);
void setUniform1f(uint programID, const std::string& name, float value);
void setUniform2v(uint programID, const std::string& name, const glm::vec2& value);
void setUniform3v(uint programID, const std::string& name, const glm::vec3& value);
void setUniform4v(uint programID, const std::string& name, const glm::vec4& value);
void setUniform4m(uint programID, const std::string& name, const glm::mat4& matrix);
