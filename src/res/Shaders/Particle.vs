#version 460 core

layout (location = 0) in vec2 aPos;   
layout (location = 1) in vec3 aOffset; // vec2 position, float alpha;

uniform mat4 projection;

out float alpha;

void main() {
    gl_Position = projection * vec4(aPos + aOffset.xy, 0.0f, 1.0f);
    alpha = aOffset.z;
}
