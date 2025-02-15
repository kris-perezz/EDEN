#ifndef __entity_h__
#define __entity_h__

#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

class entity {

    private:
        std::vector<float> verts;
        int vertCount;

    public:
        entity();
        entity(std::vector<float>);
        entity(entity&);
        float& operator[](const int index);

        void setVertCount(int);
        int getVertCount();
        void loadObj(std::string);

        float* getVertices();


};

#endif