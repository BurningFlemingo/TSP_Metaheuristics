#pragma once

#include "pch.h"
#include "VertexBufferLayout.h"

class System {
public:
    virtual ~System() {}
    virtual void draw() = 0;
    virtual void update(float dt) = 0;
};

void setProjectionMatrix(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);
glm::mat4 getProjectionMatrix();

void addVertexAttributes(VertexBufferLayout& layout);

void pushDrawCall(uint VAO);
void draw(uint programID);
