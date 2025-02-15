#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "entity.h"
#include "tiny_obj_loader.h"

void readShaders(const char* vertexPath, const char* fragmentPath,unsigned int& Shaderprogram);