#version 330 core
in vec3 Normal;
out vec4 FragColor;

void main() {
    vec3 normColor = normalize(Normal) * 0.5 + 0.5; // Convert [-1,1] to [0,1] for color display
    FragColor = vec4(normColor, 1.0);
}
