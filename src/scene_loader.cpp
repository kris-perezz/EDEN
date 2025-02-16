#include "scene_loader.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>  // C++17 for checking file existence

using json = nlohmann::json;
namespace fs = std::filesystem;  // Shorten namespace usage

std::vector<Entity> SceneLoader::loadScene(const std::string& filename) {
    std::vector<Entity> entities;
    std::ifstream file(filename);

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!file.is_open()) {
        std::cerr << "❌ Failed to open JSON file: " << filename << std::endl;
        return entities;
    }

    json sceneData;
    file >> sceneData;

    for (const auto& obj : sceneData) {
        std::string type = obj["type"];
        glm::vec3 position(
            obj["position"][0], 
            obj["position"][1], 
            obj["position"][2]
        );

        // Construct the expected file path
        std::string modelPath = "obj/" + type + ".obj";  

        // ✅ **Check if the OBJ file exists**
        if (!fs::exists(modelPath)) {
            std::cerr << "⚠️ Warning: OBJ file not found for type '" << type 
                      << "' at path: " << modelPath << std::endl;
            continue;  // Skip this object and continue loading the rest
        }

        // Create an entity
        Entity newEntity = Entity::createEntity();
        newEntity.loadObj(modelPath);
        newEntity.transform.scale = glm::vec3(2.0f, 2.0f, 2.0f); // Make objects larger for visibility
        newEntity.transform.translation = position;

        entities.push_back(std::move(newEntity));

        std::cout << "🟢 Loaded entity: " << type 
          << " at position (" << position.x << ", " << position.y << ", " << position.z << ")\n";

    }

    return entities;
}
