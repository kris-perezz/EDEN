#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <vector>
#include "entity.h"

class SceneLoader {
public:
    static std::vector<Entity> loadScene(const std::string& filename);
};

#endif // SCENE_LOADER_H
