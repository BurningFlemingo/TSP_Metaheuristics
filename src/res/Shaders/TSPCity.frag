#version 460 core

uniform sampler2D window;
in vec2 UVs;

out vec4 o_FragColor;

void main() {
    o_FragColor = texture(window, UVs);
}
