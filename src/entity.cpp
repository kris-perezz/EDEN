#include "entity.h"

entity::entity() {
    this->vertCount = verts.size();

}

entity::entity(std::vector<float> verts) {
    this->verts = verts;
    this->vertCount = verts.size();
}

entity::entity(entity &copy) {
    this->vertCount = copy.getVertCount();
    //glm::vec3 newverts[vertCount];
    //std::vector<glm::vec3> newVec;
    //this->verts = newVec;

    for(int i = 0; i < this->vertCount; i++ ) {
        this->verts.push_back( copy[i]);
    }
}

float& entity::operator[](const int index) {
    return this->verts[index];
}

float* entity::getVertices() {
    return this->verts.data();
}

int entity::getVertCount() {
    return this->vertCount;
}