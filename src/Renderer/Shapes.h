#pragma once

#include "pch.h"
#include "Renderer.h"

#define CENTER 0

struct Triangle {
    glm::vec2 p1, p2, p3;
};

/*
    quad grows from the origin right and down, this behaviour can be reversed by inputing a negative width or height
    be aware that fliping the width or height will also change the origin respectivly
*/
struct Quad {
    glm::vec2 origin;
    float w, h;
};

// pivot refers to the vertex to be pivoted around, 1-3, and 0 refers to the center of the shape
void drawTriangle(uint programID, Triangle triangle, float rotation = 0, uint pivot = 0, uint drawMethod = GL_STATIC_DRAW);

uint createQuad(uint programID, Quad quad, float rotation = 0, uint pivot = 0, uint drawMethod = GL_STATIC_DRAW);
void drawQuad(uint programID, uint VAO, Quad quad, float rotation = 0, uint pivot = 0, uint drawMethod = GL_STATIC_DRAW);
