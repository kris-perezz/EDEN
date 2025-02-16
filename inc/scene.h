#pragma once

#include "config.h"

class scene {
private:
    /* data */
    // a vector of entities
    std::vector<entity> entities;

public:
    scene(std::vector<entity>& entities);
    //~scene();//bleh destructor, i shall not implement it

    entity* getEntities();
};