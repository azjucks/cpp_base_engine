#include <time_manager.h>
using namespace Core;

#include <GLFW/glfw3.h>

void TimeManager::updateTime()
{
	newTime = glfwGetTime();
	deltaTimeUnscaled = (float)(newTime - oldTime);
	oldTime = newTime;

	deltaTime = deltaTimeUnscaled * timeScale;
}


void TimeManager::updateTimePhysics()
{
	deltaTimeStock += deltaTime;
}

bool TimeManager::fixedDeltaLoop()
{
	if (deltaTimeStock > fixedDeltaTime)
	{
		deltaTimeStock -= fixedDeltaTime;
		return true;
	}

	return false;
}