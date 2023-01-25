#include "Shapes.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Shader.h"
#include "../Core/Debug.h"
#include "../Utils.h"

void drawTriangle(uint programID, Triangle triangle, float rotation, uint pivot, uint drawMethod) {
    float vertices[] = {
        triangle.p1.y, triangle.p1.y,
        triangle.p2.x, triangle.p2.y,
        triangle.p3.x, triangle.p3.y
    };
    uint indices[] = {
        0, 1, 2
    };

    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, drawMethod);

    VertexBufferLayout layout;
    addVertexBufferElement(layout, GL_UNSIGNED_INT, 2);
    addVertexAttributes(layout);

    setUniform4m(programID, "projection", getProjectionMatrix());

    uint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 3, indices, drawMethod);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    pushDrawCall(VAO);
}

uint createQuad(uint programID, Quad quad, float rotation, uint pivot, uint drawMethod) {
    int vertices[] = {
        0, 0,  // bottom left
        1 , 0,  // bottom right
        0, 1,  // top left
        1, 1  // top right
    };
    uint indices[] = {
        0, 1, 2,
        2, 3, 1
    };

    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, drawMethod);

    VertexBufferLayout layout;
    addVertexBufferElement(layout, GL_INT, 2);
    addVertexAttributes(layout);

    uint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint), indices, drawMethod);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(quad.origin.x, quad.origin.y, 0.0f));  

    modelPivotSquare(model, rotation, quad.w, quad.h, pivot);

    model = glm::scale(model, glm::vec3(quad.w, quad.h, 1.0f));

    setUniform4m(programID, "model", model);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return VAO;
}

void drawQuad(uint programID, uint VAO, Quad quad, float rotation, uint pivot, uint drawMethod) {
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(quad.origin.x, quad.origin.y, 0.0f));  

    modelPivotSquare(model, rotation, quad.w, quad.h, pivot);

    model = glm::scale(model, glm::vec3(quad.w, quad.h, 1.0f));

    setUniform4m(programID, "model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
