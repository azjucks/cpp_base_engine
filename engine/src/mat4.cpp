#include <mat4.h>

#include <maths.h>
using namespace Core::Maths;

mat4 Core::Maths::identity()
{
    return {
           1.f, 0.f, 0.f, 0.f,
           0.f, 1.f, 0.f, 0.f,
           0.f, 0.f, 1.f, 0.f,
           0.f, 0.f, 0.f, 1.f
    };
}

mat4 Core::Maths::translate(vec3 position)
{
    return { 1.f,  0.f,  0.f, 0.f,
             0.f,  1.f,  0.f, 0.f,
             0.f,  0.f,  1.f, 0.f,
             position.x,  position.y,  position.z, 1.f };
}

mat4 Core::Maths::scale(vec3 scale)
{
    return {
        scale.x,     0.f,     0.f, 0.f,
        0.f,     scale.y,     0.f, 0.f,
        0.f,         0.f, scale.z, 0.f,
        0.f,         0.f,     0.f, 1.f,
    };
}

mat4 Core::Maths::rotateX(float angleRadians)
{
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);

    return { 1.f,  0.f,  0.f,  0.f,
             0.f,    c,    s,  0.f,
             0.f,   -s,    c,  0.f,
             0.f,  0.f,  0.f,  1.f };
}

mat4 Core::Maths::rotateY(float angleRadians)
{
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);

    return {   c,  0.f,   -s,  0.f,
             0.f,  1.f,  0.f,  0.f,
               s,  0.f,    c,  0.f,
             0.f,  0.f,  0.f,  1.f };
}

mat4 Core::Maths::rotateZ(float angleRadians)
{
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);

    return {   c,    s,  0.f,  0.f,
              -s,    c,  0.f,  0.f,
             0.f,  0.f,  1.f,  0.f,
             0.f,  0.f,  0.f,  1.f };
}

mat4 Core::Maths::frustum(float left, float right, float bottom, float top, float near, float far)
{
    float a = (2 * near) / (right - left);
    float b = (2 * near) / (top - bottom);
    float c = (right + left) / (right - left);
    float d = (top + bottom) / (top - bottom);
    float e = (far + near) / (far - near);
    float f = (2 * far * near) / (far - near);

    return {
          a, 0.f,  0.f,  0.f,
        0.f,   b,  0.f,  0.f,
          c,   d,   -e, -1.f,
        0.f, 0.f,   -f,  0.f
    };
}

mat4 Core::Maths::ortho(float left, float right, float bottom, float top, float near, float far)
{
    float a = 2 / (right - left);
    float b = 2 / (top - bottom);
    float c = -2 / (far - near);
    float d = -(right + left) / (right - left);
    float e = -(top + bottom) / (top - bottom);
    float f = -(far + near) / (far - near);
    return {
          a, 0.f, 0.f, 0.f,
        0.f,   b, 0.f, 0.f,
        0.f, 0.f,   c, 0.f,
          d,   e,   f, 1.f
    };
}

mat4 Core::Maths::perspective(float fovY, float aspect, float near, float far)
{
    float top = near * tanf(fovY * 0.5f);
    float right = top * aspect;
    return frustum(-right, right, -top, top, near, far);
}

mat4 Core::Maths::orthographic(float fovY, float aspect, float near, float far)
{
    float top = fovY;
    float right = top * aspect;
    return ortho(-right, right, -top, top, near, far);
}

/*
vec4 Core::Maths::mat4::operator*(vec4 v) const
{
    vec4 pos;

    for (int i = 0; i < 4; ++i)
    {
        float sum = 0.0f;

        for (int j = 0; j < 4; ++j)
        {
            sum += v.x * this->c[j].e[i];
        }

        pos.e[i] = sum;
    }

    return pos;
}*/

mat4 mat4::operator*(const mat4& v) const
{
    return matrixMultiply(*this, v);
}

