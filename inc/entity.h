#ifndef __entity_h__
#define __entity_h__

#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

class entity {

    private:
        std::vector<glm::vec3> verts;
        int triCount;

    public:
        entity();
        entity(std::vector<glm::vec3>);
        entity(entity&);
        glm::vec3& operator[](const int index);

        void translateEntity(glm::vec4);
        void rotateEntityX(float);
        void scaleEntity(glm::vec4);
        void rotateEntityY(float);
        void rotateEntityZ(float);

        void setTriCount(int);
        int getTriCount();
        void loadObj(std::string);

        glm::vec3* getVertices();
        void depthTest(int WIDTH,int HEIGHT,int &count, u_int32_t* frameBuffer, float* depthBuffer,std::vector<float> facingRatios);
        void applyWave();




};

#endif