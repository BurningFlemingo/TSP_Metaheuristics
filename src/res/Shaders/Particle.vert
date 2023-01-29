#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aOffset;  // vec3 position, float rotation

uniform mat4 projection;

float PI = 3.141592;
float radians = aOffset[3];

mat4 translate(mat4 m, vec3 v) {
    mat4 matrix = mat4(1.0f);

	matrix[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

    return matrix;
}

mat2 rotate(float deg) {
    float rad = (deg * PI) / 180;

    float s = sin(rad);
    float c = cos(rad);
    return mat2(
    c, -s,
    s, c);
}

void main() {
    vec2 quad = vec2(aPos);
    quad = rotate(radians) * quad;
    
    gl_Position = projection * vec4(quad + aOffset.xy, 0.0f, 1.0f);
}
