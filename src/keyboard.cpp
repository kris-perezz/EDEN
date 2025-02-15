#include "keyboard.h"

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, Entity& Object) {
    glm::vec3 rotate{0.0f};

    if(glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
    if(glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;

    if(glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
    if(glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;


    //to avoid float comparison with zero
    if(glm::dot(rotate,rotate) > std::numeric_limits<float>::epsilon()) {
        Object.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
    }

    Object.transform.rotation.x = glm::clamp(Object.transform.rotation.x, -1.5f, 1.5f);
    Object.transform.rotation.y = glm::mod(Object.transform.rotation.y, glm::two_pi<float>());

    float yaw = Object.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, 1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

    //to avoid float comparison with zero
    if(glm::dot(moveDir,moveDir) > std::numeric_limits<float>::epsilon()) {
        Object.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
}