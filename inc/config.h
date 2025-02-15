#pragma once
#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include "tiny_obj_loader.h"
#include "entity.h"
#include "camera.h"



void readShaders(const char* vertexPath, const char* fragmentPath,unsigned int& Shaderprogram);