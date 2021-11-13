#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

struct Vec2
{
	float x , y;
};

struct Vec3
{
	float x , y , z;
};

struct Vertex
{
	Vec3 position;
	Vec2 texCord;
	Vec3 normal;
};

const unsigned int MAX_POINTLIGHT_COUNT = 3;