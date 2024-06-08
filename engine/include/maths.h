#pragma once
#include <cmath>

#include <vec2.h>
#include <vec3.h>
#include <vec4.h>



namespace Core
{
	namespace Maths
	{
#ifndef PI
#define PI 3.14159265358979323846f
#endif
#ifndef TAU
#define TAU 6.283185307179586476925f
#endif

#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)

		inline float clamp(float x, float min, float max)
		{
			if (x < min)
				return min;
			if (x > max)
				return max;
			return x;
		}

		inline vec2 clamp(vec2 v, float min, float max)
		{
			return { Maths::clamp(v.x, min, max),
					 Maths::clamp(v.y, min, max) };
		}

		inline vec3 clamp(vec3 v, float min, float max)
		{
			return { Maths::clamp(v.x, min, max),
					 Maths::clamp(v.y, min, max),
					 Maths::clamp(v.z, min, max) };
		}

		inline vec4 clamp(vec4 v, float min, float max)
		{
			return { Maths::clamp(v.x, min, max),
					 Maths::clamp(v.y, min, max),
					 Maths::clamp(v.z, min, max),
					 Maths::clamp(v.w, min, max) };
		}

		inline float getMin(float x, float y)
		{
			return x < y ? x : y;
		}

		inline float getMin(float x, float y, float z)
		{
			return getMin(getMin(x, y), z);
		}

		inline float getMin(const vec3& v)
		{
			return getMin(v.x, v.y, v.z);
		}

		inline float getMax(float x, float y)
		{
			return x > y ? x : y;
		}

		// ===============
		// Maths Utilities

		inline float linear_conversion(float value, float oldMin, float oldMax, float newMin, float newMax)
		{
			return (value - oldMin) * (newMax - newMin) / (oldMax - oldMin) + newMin;
		}

		template <typename T>
		inline T lerp(T start, T end, float percent)
		{
			return start + (end - start) * percent;
		}
	}
}
