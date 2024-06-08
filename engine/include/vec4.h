#pragma once

#include <cmath>

namespace Core
{
	namespace Maths
	{
		union vec4
		{
			struct {
				float x, y, z, w;
			};
			float e[4];

			inline vec4 operator-() const
			{
				return { -x, -y, -z, -w };
			}

			inline vec4 operator+(const float& f) const
			{
				return { x + f, y + f, z + f, w + f };
			}

			inline vec4 operator+(const vec4& v) const
			{
				return { x + v.x, y + v.y, z + v.z, w + v.w };
			}

			inline vec4 operator-(const vec4& v) const
			{
				return { x - v.x, y - v.y, z - v.z, w - v.w };
			}

			inline vec4 operator*(float n) const
			{
				return { x * n, y * n, z * n, w * n };
			}

			inline vec4 operator/(float n) const
			{
				return { x / n, y / n, z / n, w / n };
			}

			inline vec4& operator+=(const vec4& v)
			{
				*this = *this + v;
				return *this;
			}

			inline vec4& operator-=(const vec4& v)
			{
				*this = *this - v;
				return *this;
			}

			inline vec4& operator*=(const float& n)
			{
				*this = *this * n;
				return *this;
			}

			inline vec4& operator/=(const float& n)
			{
				*this = *this / n;
				return *this;
			}
		};

		inline float dot_product(const vec4& v1, const vec4& v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
		}

		inline float length(const vec4& v)
		{
			return sqrtf(dot_product(v, v));
		}

		inline float length_sqr(const vec4& v)
		{
			return dot_product(v, v);
		}

		inline vec4 normalized(const vec4& v)
		{
			float len = length(v);
			if (len < 0.0001f)
				return { 0, 0, 0, 0 };
			return v / len;
		}
	}
}
