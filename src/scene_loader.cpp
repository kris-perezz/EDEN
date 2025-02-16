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

        std::string modelPath = "obj/" + type + ".obj";  

        if (!fs::exists(modelPath)) {
            std::cerr << "⚠️ Warning: OBJ file not found for type '" << type 
                      << "' at path: " << modelPath << std::endl;
            continue;
        }

        Entity newEntity = Entity::createEntity();
        newEntity.loadObj(modelPath);
        newEntity.transform.translation = position;

        // ✅ Load color if it exists
        if (obj.contains("colour")) {
            newEntity.setColour(obj["colour"][0], obj["colour"][1], obj["colour"][2]);
        } else {
            newEntity.setColour(1.0f, 1.0f, 1.0f); // ✅ Default to white if missing
        }

        entities.push_back(std::move(newEntity));

        std::cout << "🟢 Loaded entity: " << type 
          << " at position (" << position.x << ", " << position.y << ", " << position.z 
          << ") with color (" << newEntity.getColour().r << ", " << newEntity.getColour().g 
          << ", " << newEntity.getColour().b << ")\n";
    }

    return entities;
}
