#pragma once

#include "../dependencies/glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "camera.h"
#include "entity.h"

#include "eden_tools.hpp"
#include "keyboard.h"
#include "tiny_obj_loader.h"

void readShaders(const char *vertexPath, const char *fragmentPath,
                 unsigned int &Shaderprogram);
void loadObject();
