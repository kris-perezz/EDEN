#version 330 core

in vec3 FragPos;  // Fragment position in world space
in vec3 Normal;   // Normal for lighting calculations

out vec4 FragColour;

uniform vec3 lightPos; // Light source position
uniform vec3 viewPos;  // Camera position
uniform vec3 lightColour;
uniform vec3 objectColour; // Colour of the object

void main()
{
    // Normalize inputs
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Ambient lighting (constant low-intensity light)
    vec3 ambient = 0.3 * lightColour; // Increase ambient to make objects more visible

    // Diffuse lighting (Lambertian reflection)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;

    // Specular lighting (Phong reflection)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16); // Adjust shininess with 16
    vec3 specular = 0.5 * spec * lightColour;

    // Final Colour calculation
    vec3 result = (ambient + diffuse + specular) * objectColour;
    FragColour = vec4(result, 1.0);
}
