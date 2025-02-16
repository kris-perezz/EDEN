#include "config.h"

#include <glm/gtc/type_ptr.hpp>
#include <chrono>

int main() {
    
    GLFWwindow* window;
    
    if (!glfwInit()) {
        std::cout << "GLFW couldn't start" << std::endl;
        glfwTerminate();
        return -1;
    }

    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load OpenGL" << std::endl;
		glfwTerminate();
		return -1;
	}

    glClearColor(0.5f, 0.1f, 0.75f, 1.0f);

    
    // --- Minimal modifications to load an OBJ model ---
    // Replace the hard-coded triangle with OBJ data
    std::vector<float> objVertices;
    std::vector<unsigned int> objIndices;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    // Load the OBJ file (update the path accordingly)
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "obj/cesar.obj");
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Failed to load OBJ file" << std::endl;
        return -1;
    }
    
    // For each shape and each index, extract the vertex positions.
    for (size_t s = 0; s < shapes.size(); s++) {
        for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
            tinyobj::index_t idx = shapes[s].mesh.indices[i];
            // Assuming the OBJ file has positions only (x, y, z)
            objVertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            objVertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            objVertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
            // Build a sequential index array
            objIndices.push_back(static_cast<unsigned int>(i));
        }
    }
    
    
    //entity importedModel(objVertices,objIndices);
    
    
    //std::cout << "Size: " << importedModel.getVertCount();
    

    // --- End of OBJ loading modifications ---

    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);


    // Bind and fill VBO with OBJ vertex data

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  

    glBufferData(GL_ARRAY_BUFFER, objVertices.size() * sizeof(float), objVertices.data(), GL_STATIC_DRAW);
    
    // Bind and fill EBO with OBJ index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objIndices.size() * sizeof(unsigned int),  objIndices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers (location 0 expects a vec3 position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    unsigned int shaderProgram;
    readShaders("./shaders/shader.vert","./shaders/shader.frag", shaderProgram);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // --- Camera Setup ---
    Camera camera;
    // Configure perspective projection: 45° FOV, correct aspect, near and far clipping planes
    camera.setPerspectiveProjection(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    // Set view: camera positioned at (0,0,3) looking at the origin
    camera.setViewTarget(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    // Get uniform locations from your shader
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");


    //binding uniform buffers
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    unsigned int blockIndex = glGetUniformBlockIndex(shaderProgram, "Matrices");
    glUniformBlockBinding(shaderProgram, blockIndex, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);


    auto viewerObject = Entity::createEntity();
    KeyboardMovementController cameraController{};
    auto currentTime = std::chrono::high_resolution_clock::now();


    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;


        cameraController.moveInPlaneXZ(window,frameTime,viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        // Update camera uniforms each frame (if the camera is moving, update accordingly)
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMat()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjection())); 

        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getViewMat()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);


        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(objIndices.size()), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

    }

    glfwTerminate();

    return 0;
}

// helper function to streamline reading the shaderfiles
void readShaders(const char* vertexPath, const char* fragmentPath, unsigned int& shProgram) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // shader Program
    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    shProgram=ID;

}