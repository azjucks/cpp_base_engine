#pragma once

#include <cmath>

namespace Core
{
	namespace Maths
	{
		union vec3
		{
			struct {
				float x, y, z;
			};
			float e[3];

			inline vec3& operator=(const vec3& v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				return *this;
			}

			inline bool operator==(const vec3& v) const
			{
				return x == v.x && y == v.y && z == v.z;
			}

			inline vec3 operator-() const
			{
				return { -x, -y, -z };
			}

			inline vec3 operator+(const float& f) const
			{
				return { x + f, y + f, z + f };
			}

			inline vec3 operator+(const vec3& v) const
			{
				return { x + v.x, y + v.y, z + v.z };
			}

			inline vec3 operator-(const vec3& v) const
			{
				return { x - v.x, y - v.y, z - v.z };
			}

			inline vec3 operator*(float n) const
			{
				return { x * n, y * n, z * n };
			}

			inline vec3 operator/(float n) const
			{
				return { x / n, y / n, z / n };
			}

			inline vec3& operator+=(const vec3& v)
			{
				*this = *this + v;
				return *this;
			}

			inline vec3& operator-=(const vec3& v)
			{
				*this = *this - v;
				return *this;
			}

			inline vec3& operator*=(const float& n)
			{
				*this = *this * n;
				return *this;
			}

			inline vec3& operator/=(const float& n)
			{
				*this = *this / n;
				return *this;
			}
		};

		inline vec3 abs(const vec3& v)
		{
			return { fabsf(v.x), fabsf(v.y), fabsf(v.z) };
		}

		inline float dot_product(const vec3& v1, const vec3& v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}

		inline vec3 cross_product(const vec3& v1, const vec3& v2)
		{
			return { v1.y * v2.z - v1.z * v2.y,
					 v1.z * v2.x - v1.x * v2.z,
					 v1.x * v2.y - v1.y * v2.x };
		}

		inline float length(const vec3& v)
		{
			return sqrtf(dot_product(v, v));
		}

		inline float length_sqr(const vec3& v)
		{
			return dot_product(v, v);
		}

		inline float angle(const vec3& v1, const vec3& v2)
		{
			return acosf(dot_product(v1, v2));
		}

		inline vec3 normalized(const vec3& v)
		{
			float len = length(v);

			if (len < 0.0001f)
				return { 0, 0, 0 };

			return v / len;
		}
	}
}
