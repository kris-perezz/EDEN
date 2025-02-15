#ifndef __entity_h__
#define __entity_h__

#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

#include "camera.h"

class entity {

    private:
        std::vector<float> verts;
        std::vector<unsigned int> indicies;

        int vertCount;
        unsigned int indexCount;

    public:
        entity();
        entity(std::vector<float>);
        entity(std::vector<float> verts, std::vector<unsigned int> indicies);
        entity(entity&);
        float& operator[](const int index);

        void setVertCount(int);
        int getVertCount();
        int getIndexCount();

        void loadObj(std::string);

        float* getVertices();
        unsigned int* getIndices();



};

#endif