#ifndef __camera_h__
#define __camera_h__

// forces radians across all platforms
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // maybe change later
#include <cassert>
#include <glm/glm.hpp>
#include <limits>

class Camera {

public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw = -90.0f;
  float pitch = 0.0f;

  float sensitivity = 0.1f;
  float moveSpeed = 1.50f;

  // Constructor
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
      : position(position), worldUp(up) {
    updateVectors();
  }

  inline glm::mat4 getProjectionMatrix() { return this->projectionMatrix; }

  inline glm::mat4 getViewMatrix() {
    return glm::lookAt(position, position + front, up);
  }

  void processMouseMovement(float xoffset, float yoffset);

  void processWASDInput();

  void setOrthographicProjection(float left, float right, float top,
                                 float bottom, float near, float far);
  void setPerspectiveProjection(float FOVy, float aspectRatio, float near,
                                float far);
  /*void setViewDirection(glm::vec3 position, glm::vec3 direction,*/
  /*                      glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});*/
  /*void setViewTarget(glm::vec3 position, glm::vec3 target,*/
  /*                   glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});*/
  /**/
  /*void setViewYXZ(glm::vec3 position, glm::vec3 rotation);*/

private:
  glm::mat4 projectionMatrix{1.0f};

  void updateVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }
};

#endif
