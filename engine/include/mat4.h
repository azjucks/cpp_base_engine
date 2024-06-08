#pragma once

#include <maths.h>

namespace Core
{
	namespace Maths
	{
		union mat4
		{
			/*
			struct {
				float m0, m4, m8, m12;
				float m1, m5, m9, m13;
				float m2, m6, m10, m14;
				float m3, m7, m11, m15;
			};
			*/

			float e[16] = { 0 };
			vec4 c[4];

			//vec4 operator*(vec4 v) const;
			mat4 operator*(const mat4& v) const;
		};

		inline mat4 matrixMultiply(const mat4& a, const mat4& b)
		{
			mat4 mat = {};

			for (int c = 0; c < 4; ++c)
			{
				for (int r = 0; r < 4; ++r)
				{
					for (int k = 0; k < 4; ++k)
					{
						mat.c[c].e[r] += a.c[k].e[r] * b.c[c].e[k];
					}
				}
			}

			return mat;
			/*
			mat4 result = { 0 };

			result.m0 = a.m0 * b.m0 + a.m1 * b.m4 + a.m2 * b.m8 + a.m3 * b.m12;
			result.m1 = a.m0 * b.m1 + a.m1 * b.m5 + a.m2 * b.m9 + a.m3 * b.m13;
			result.m2 = a.m0 * b.m2 + a.m1 * b.m6 + a.m2 * b.m10 + a.m3 * b.m14;
			result.m3 = a.m0 * b.m3 + a.m1 * b.m7 + a.m2 * b.m11 + a.m3 * b.m15;
			result.m4 = a.m4 * b.m0 + a.m5 * b.m4 + a.m6 * b.m8 + a.m7 * b.m12;
			result.m5 = a.m4 * b.m1 + a.m5 * b.m5 + a.m6 * b.m9 + a.m7 * b.m13;
			result.m6 = a.m4 * b.m2 + a.m5 * b.m6 + a.m6 * b.m10 + a.m7 * b.m14;
			result.m7 = a.m4 * b.m3 + a.m5 * b.m7 + a.m6 * b.m11 + a.m7 * b.m15;
			result.m8 = a.m8 * b.m0 + a.m9 * b.m4 + a.m10 * b.m8 + a.m11 * b.m12;
			result.m9 = a.m8 * b.m1 + a.m9 * b.m5 + a.m10 * b.m9 + a.m11 * b.m13;
			result.m10 = a.m8 * b.m2 + a.m9 * b.m6 + a.m10 * b.m10 + a.m11 * b.m14;
			result.m11 = a.m8 * b.m3 + a.m9 * b.m7 + a.m10 * b.m11 + a.m11 * b.m15;
			result.m12 = a.m12 * b.m0 + a.m13 * b.m4 + a.m14 * b.m8 + a.m15 * b.m12;
			result.m13 = a.m12 * b.m1 + a.m13 * b.m5 + a.m14 * b.m9 + a.m15 * b.m13;
			result.m14 = a.m12 * b.m2 + a.m13 * b.m6 + a.m14 * b.m10 + a.m15 * b.m14;
			result.m15 = a.m12 * b.m3 + a.m13 * b.m7 + a.m14 * b.m11 + a.m15 * b.m15;

			return result;
			*/
		}


		mat4 identity();
		mat4 translate(vec3 position);
		mat4 scale(vec3 scale);
		mat4 rotateX(float angleRadians);
		mat4 rotateY(float angleRadians);
		mat4 rotateZ(float angleRadians);
		mat4 frustum(float left, float right, float bottom, float top, float near, float far);
		mat4 ortho(float left, float right, float bottom, float top, float near, float far);
		mat4 perspective(float fovY, float aspect, float near, float far);
		mat4 orthographic(float fovY, float aspect, float near, float far);
	}
}