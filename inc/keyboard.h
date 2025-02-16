#ifndef __keyboard_h__
#define __keyboard_h__

#include "config.h"
class KeyboardMovementController {



    public:

    struct KeyMappings {
        int moveLeft = GLFW_KEY_LEFT;
        int moveRight = GLFW_KEY_RIGHT;
        int moveForward = GLFW_KEY_E;
        int moveBackward = GLFW_KEY_Q;
        int moveUp = GLFW_KEY_UP;
        int moveDown = GLFW_KEY_DOWN;
        int lookLeft = GLFW_KEY_A;
        int lookRight = GLFW_KEY_D;
        int lookUp = GLFW_KEY_W;
        int lookDown = GLFW_KEY_S;
    };

    void moveInPlaneXZ(GLFWwindow* window, float dt, Entity& Object);

    KeyMappings keys{};
    float moveSpeed{3.0f};
    float lookSpeed{1.5};

};


#endif