#pragma once

#include <singleton.h>

namespace Core
{
	class TimeManager : public Singleton<TimeManager>
	{
		friend class Singleton<TimeManager>;

	private:
		double oldTime = 0.0;
		double newTime = 0.0;

		float deltaTimeStock = 0.f;

	public:
		float deltaTimeUnscaled = 0.f;
		float deltaTime = 0.f;
		float fixedDeltaTime = 0.02f;
		float timeScale = 1.0f;

		void updateTime();
		void updateTimePhysics();

		bool fixedDeltaLoop();
	};
}