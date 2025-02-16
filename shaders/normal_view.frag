#version 330 core
in vec3 Normal;   // Normal from vertex shader
out vec4 FragColor;

void main() {
    vec3 normalColor = normalize(Normal) * 0.5 + 0.5; // Normalize to [0,1] range
    FragColor = vec4(normalColor, 1.0);
}
