#pragma once

#include "../dependencies/glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "camera.h"
#include "entity.h"

#include "eden_tools.hpp"
#include "keyboard.h"
#include "scene_loader.h"
#include "tiny_obj_loader.h"

#include <chrono>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
void readShaders(const char *vertexPath, const char *fragmentPath,
                 unsigned int &Shaderprogram);
