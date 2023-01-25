#pragma once

#include "pch.h"

struct VertexBufferElement {
    uint type;
    uint count;
    uint normalized;

    static uint getSizeOfType(uint type) {
        switch(type) {
            case GL_UNSIGNED_INT: {
                return sizeof(GLuint);
                break;
            }
            case GL_INT: {
                return sizeof(GLint);
                break;
            }
            case GL_FLOAT: {
                return sizeof(GLfloat);
                break;
            }
            default: {
                return 0;
                break;
            }
        }
    }
};

struct VertexBufferLayout {
    VertexBufferLayout() {
        stride = 0;
    }
    std::vector<VertexBufferElement> elements;
    uint stride;
};

void addVertexBufferElement(VertexBufferLayout& layout, uint type, uint count, bool normalized = GL_FALSE);
