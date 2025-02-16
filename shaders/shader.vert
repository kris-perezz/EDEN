#version 330 core

// Input attributes from VBO (layout locations match `glVertexAttribPointer`)
layout(location = 0) in vec3 aPos;   // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

// Output to Fragment Shader
out vec3 FragPos;
out vec3 Normal;

// Uniforms (provided by OpenGL)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0)); // Transform position to world space
    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normal to world space

    gl_Position = projection * view * model * vec4(aPos, 1.0); // Transform position to clip space
}
