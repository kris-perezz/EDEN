#include "scene.h"

scene::scene(std::vector<entity>& entities) {
    this->entities = entities;

}

entity* scene::getEntities() {
    return this->entities.data();
}
