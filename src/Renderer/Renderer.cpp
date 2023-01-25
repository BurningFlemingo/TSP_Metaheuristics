#include "pch.h"
#include "Renderer.h"
#include "Shader.h"
#include "../Core/Debug.h"

glm::mat4 proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);
std::vector<uint> drawCalls;

void setProjectionMatrix(float left, float right, float bottom, float top, float near, float far) {
    proj = glm::ortho(left, right, bottom, top, near, far);
}

glm::mat4 getProjectionMatrix() {
    return proj;
}

void addVertexAttributes(VertexBufferLayout& layout) {
    uint offset = 0;

    for (int i = 0; i < layout.elements.size(); i++) {
        VertexBufferElement element = layout.elements[i];

        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride, (void*)(uintptr_t)(offset));
        glEnableVertexAttribArray(i);

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void pushDrawCall(uint VAO) {
    drawCalls.emplace_back(VAO);
}

void draw(uint programID) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto object : drawCalls) {
        glBindVertexArray(object);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    drawCalls.clear();
}
