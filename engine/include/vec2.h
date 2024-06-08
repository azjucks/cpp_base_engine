#pragma once

#include <cmath>

namespace Core
{
	namespace Maths
	{
		union vec2
		{
			struct {
				float x, y;
			};
			float e[2];

			inline vec2 operator-() const
			{
				return { -x, -y };
			}

			inline vec2 operator+(const float& f) const
			{
				return { x + f, y + f};
			}

			inline vec2 operator+(const vec2& v) const
			{
				return { x + v.x, y + v.y };
			}

			inline vec2 operator-(const vec2& v) const
			{
				return { x - v.x, y - v.y };
			}

			inline vec2 operator*(float n) const
			{
				return { x * n, y * n };
			}

			inline vec2 operator/(float n) const
			{
				return { x / n, y / n };
			}

			inline vec2& operator+=(const vec2& v)
			{
				*this = *this + v;
				return *this;
			}
			inline vec2& operator-=(const vec2& v)
			{
				*this = *this - v;
				return *this;
			}
			inline vec2& operator*=(const float& n)
			{
				*this = *this * n;
				return *this;
			}
			inline vec2& operator/=(const float& n)
			{
				*this = *this / n;
				return *this;
			}			
		};



		inline float dot_product(const vec2& v1, const vec2& v2)
		{
			return v1.x * v2.x + v1.y * v2.y;
		}

		inline float length(const vec2& v)
		{
			return sqrtf(dot_product(v, v));
		}

		inline float length_sqr(const vec2& v)
		{
			return dot_product(v, v);
		}

		inline vec2 normalized(const vec2& v)
		{
			float len = length(v);
			if (len < 0.0001f)
				return { 0, 0 };
			return v / len;
		}
	}
}
