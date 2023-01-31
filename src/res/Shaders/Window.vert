#version 460 core

layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec2 i_UVs;

uniform float scale;

out vec2 UVs;

void main() {
    gl_Position = vec4(i_Pos * scale, 1.0f);
    UVs = i_UVs;
}
