#version 330 core
layout (location = 0) in vec3 aPos;


// Define a Uniform Buffer for Matrices
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}