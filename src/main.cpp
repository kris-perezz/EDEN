#include "config.h"

#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>


// ✅ Define `useNormalShader` globally
bool useNormalShader = false; 
unsigned int shaderProgram; 
unsigned int normalShaderProgram; 

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        useNormalShader = !useNormalShader;  // ✅ No extern needed

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ✅ Add slight delay
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // ✅ Resize OpenGL viewport
}


int main() {
    
    GLFWwindow* window;
    
    
    if (!glfwInit()) {
        std::cout << "GLFW couldn't start" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // ✅ Allow resizing
    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load OpenGL" << std::endl;
		glfwTerminate();
		return -1;
	}
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  // Cull back faces
    glFrontFace(GL_CW);  // Ensure counter-clockwise faces are front
    glEnable(GL_DEPTH_TEST);


    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Load scene from JSON
    std::vector<Entity> sceneObjects = SceneLoader::loadScene("scene.json");



    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // ✅ Load the regular shader
    readShaders("./shaders/shader.vert", "./shaders/shader.frag", shaderProgram);

    // ✅ Load the normal visualization shader
    readShaders("./shaders/shader.vert", "./shaders/normal_view.frag", normalShaderProgram);


    glfwSetKeyCallback(window, keyCallback);

    // --- Camera Setup ---
    Camera camera;
    // Configure perspective projection: 45° FOV, correct aspect, near and far clipping planes
    camera.setPerspectiveProjection(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    // Set view: camera positioned at (0,0,3) looking at the origin
    camera.setViewTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

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

    // Create an actual object instead of an R-value
    Entity viewerObject = Entity::createEntity();

    // Ensure transform component exists
    viewerObject.transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);

    KeyboardMovementController cameraController{};
    auto currentTime = std::chrono::high_resolution_clock::now();


   if (shaderProgram == 0 || normalShaderProgram == 0) {
    std::cerr << "❌ ERROR: Shader failed to load!" << std::endl;
}

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
    
    
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;
        
        //std::cout << "📷 Camera Position: " 
            //<< viewerObject.transform.translation.x << ", " 
            //<< viewerObject.transform.translation.y << ", " 
           // << viewerObject.transform.translation.z << std::endl;
    
        // Move the camera
        cameraController.moveInPlaneXZ(window, frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
    
        // ✅ Make the light rotate around the origin (0,0,0)
        float timeValue = glfwGetTime();
        float radius = 8.0f; // Distance from the origin
        glm::vec3 lightPos = glm::vec3(
            cos(timeValue) * radius,  // X coordinate (rotates around Y-axis)
            0.0f,                     // Fixed height (Y-coordinate)
            sin(timeValue) * radius   // Z coordinate (rotates around Y-axis)
        );
    
        glm::vec3 lightColour = glm::vec3(2.0f, 2.0f, 2.0f); // Increase brightness
        glUniform3f(glGetUniformLocation(shaderProgram, "lightColour"), lightColour.x, lightColour.y, lightColour.z);

        // Pass updated light position to the shader
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
        // Pass camera position to the shader
        glm::vec3 cameraPos = viewerObject.transform.translation;
        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
    
        // Set object color (modify per entity if needed)
        glm::vec3 objectColour = glm::vec3(1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColour"), objectColour.x, objectColour.y, objectColour.z);
        
        // Pass matrices to shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMat()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));
        
        //std::cout << "💡 Light Position: " 
          //<< lightPos.x << ", " 
          //<< lightPos.y << ", " 
          //<< lightPos.z << std::endl;

        // Clear buffers and draw scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        

        for (auto& entity : sceneObjects) {
            glUseProgram(useNormalShader ? normalShaderProgram : shaderProgram);
        
            glm::mat4 model = entity.transform.mat4();
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        
            // ✅ Send entity color to shader
            GLint colourLoc = glGetUniformLocation(shaderProgram, "objectColour");
            glUniform3f(colourLoc, entity.getColour().r, entity.getColour().g, entity.getColour().b);
        
            entity.draw();
        }
        
        
    
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