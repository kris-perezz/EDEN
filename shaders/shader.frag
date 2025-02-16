#version 330 core

// Inputs from Vertex Shader
in vec3 FragPos;
in vec3 Normal;

// Output color
out vec4 FragColor;

// Light properties
uniform vec3 lightPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(1.0, 0.5, 0.31);

// Camera position (needed for specular lighting)
uniform vec3 viewPos;

void main() {
    // Normalize interpolated normal
    vec3 norm = normalize(Normal);
    
    // Compute light direction
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Specular shading (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    // Combine results
    vec3 ambient = 0.1 * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
