#version 330 core

in float alpha;
out vec4 FragColor;
  
void main() {
    FragColor = vec4(0.0f, 1.0f, 1.0f, alpha);
}
