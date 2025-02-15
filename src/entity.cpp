#include "entity.h"

entity::entity() {
    this->triCount = verts.size();

}

entity::entity(std::vector<glm::vec3> verts) {
    this->verts = verts;
    this->triCount = verts.size();
}

entity::entity(entity &copy) {
    this->triCount = copy.getTriCount();
    //glm::vec3 newTris[triCount];
    //std::vector<glm::vec3> newVec;
    //this->verts = newVec;

    for(int i = 0; i < this->triCount; i++ ) {
        this->verts.push_back( copy[i]);
    }
}

glm::vec3& entity::operator[](const int index) {
    return this->verts[index];
}

glm::vec3* entity::getVertices() {
    return this->verts.data();
}