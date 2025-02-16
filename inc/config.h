#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "entity.h"
#include "camera.h"

#include "keyboard.h"
#include "tiny_obj_loader.h"
#include "scene_loader.h"



void readShaders(const char* vertexPath, const char* fragmentPath,unsigned int& Shaderprogram);
void loadObject( );