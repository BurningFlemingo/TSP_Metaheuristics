#include <sstream>
#include <fstream>
#include "Shader.h"

#include "../Core/Debug.h"

std::unordered_map<std::string, int> uniformLocationCache;

std::string parseShader(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string shaderString;
    if (file) {
        std::stringstream stream;
        stream << file.rdbuf();
        shaderString = stream.str();
    } else {
        log("Filepath invalid", WARNING);
    }
    file.close();

    return shaderString;
}

uint compileShader(const std::string& shaderSource, uint shaderType) {
    std::string shaderString = shaderSource;
    const char* src = shaderString.c_str();
    
    uint shaderID;
    int success;
    char infoLog[512];
       
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &src, NULL);
    glCompileShader(shaderID);

    // error handling
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        logf("Shader compilation failed", infoLog);
    }

    return shaderID;
}

uint createProgram(std::vector<uint> shaders) {
    uint programID = glCreateProgram();

    for (auto shader : shaders) {
        glAttachShader(programID, shader);
    }

    glLinkProgram(programID);

    for (auto shader : shaders) {
        glDeleteShader(shader);
    }

    return programID;
}

uint createShaders(const std::vector<std::string>& shaderLocations, const std::vector<GLuint> shaderTypes) {
    std::vector<std::string> shaderStrings;
    std::vector<uint> shaderIDs;
    uint programID;

    for (const auto& location : shaderLocations) {
        shaderStrings.emplace_back(parseShader(location)); 
    }
    for (int i = 0; i < shaderStrings.size(); i++) {
        const char* src = shaderStrings[i].c_str();

        shaderIDs.emplace_back(compileShader(src, shaderTypes[i]));
    }

    return createProgram(shaderIDs);
}

int getUniformLocation(uint programID, const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache.at(name);
    }
    int location = glGetUniformLocation(programID, name.c_str());
    return location;
}

void setUniform1i(uint programID, const std::string& name, int value) {
    glUniform1i(getUniformLocation(programID, name), value);
}
void setUniform1f(uint programID, const std::string& name, float value) {
    glUniform1f(getUniformLocation(programID, name), value);
}
void setUniform2v(uint programID, const std::string& name, const glm::vec2& value) {
    if (sizeof(value[0]) == sizeof(int)) {
        glUniform2i(getUniformLocation(programID, name), value[0], value[1]);
    } else {
        glUniform2f(getUniformLocation(programID, name), value[0], value[1]);
    }
}
void setUniform3v(uint programID, const std::string& name, const glm::vec3& value) {
    if (sizeof(value[0]) == sizeof(int)) {
        glUniform3i(getUniformLocation(programID, name), value[0], value[1], value[2]);
    } else {
        glUniform3f(getUniformLocation(programID, name), value[0], value[1], value[2]);
    }
}
void setUniform4v(uint programID, const std::string& name, const glm::vec4& value) {
    if (sizeof(value[0]) != sizeof(int)) {
        glUniform4f(getUniformLocation(programID, name), value[0], value[1], value[2], value[3]);
    } else {
        glUniform4i(getUniformLocation(programID, name), value[0], value[1], value[2], value[3]);
    }
}
void setUniform4m(uint programID, const std::string& name, const glm::mat4& matrix) {
        glUniformMatrix4fv(getUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
