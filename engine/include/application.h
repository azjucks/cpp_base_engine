#pragma once

#include <vector>

#include <camera.h>
using namespace LowRenderer;

#include <resources_manager.h>
using namespace Resources;
#include <graph.h>
using namespace Core::DataStructure;
#include <log.h>
using namespace Core::Debug;

struct GLFWwindow;

class Application
{
private:
	GLFWwindow* window = nullptr;
	Graph* graph = nullptr;

	unsigned int scr_width;
	unsigned int scr_height;

public:
	Application() = default;
	Application(unsigned int scr_width, unsigned int scr_height);
	~Application();

	int initialize();
	void renderLoop();
};