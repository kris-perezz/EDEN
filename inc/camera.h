#ifndef __camera_h__
#define __camera_h__

//forces radians across all platforms
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //maybe change later
#include <glm/glm.hpp>
#include <cassert>
#include <limits>

class Camera {

    private:

        glm::mat4 projectionMatrix{1.0f};
        glm::mat4 viewMatrix{1.0f};

        


    public:

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float FOVy, float aspectRatio, float near, float far);
        
        const glm::mat4& getProjection() const;
        const glm::mat4& getViewMat() const;

        void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
        void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});

        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);
        


};

#endif