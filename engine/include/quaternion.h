#pragma once

#include <maths.h>
#include <mat4.h>


namespace Core
{
    namespace Maths
    {
        // Vector4 type
        typedef struct Quaternion {
            float x;
            float y;
            float z;
            float w;
        } Quaternion;

        // Add two quaternions
        inline Quaternion QuaternionAdd(Quaternion q1, Quaternion q2)
        {
            Quaternion result = { q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w };
            return result;
        }

        // Add quaternion and float value
        inline Quaternion QuaternionAddValue(Quaternion q, float add)
        {
            Quaternion result = { q.x + add, q.y + add, q.z + add, q.w + add };
            return result;
        }

        // Subtract two quaternions
        inline Quaternion QuaternionSubtract(Quaternion q1, Quaternion q2)
        {
            Quaternion result = { q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w };
            return result;
        }

        // Subtract quaternion and float value
        inline Quaternion QuaternionSubtractValue(Quaternion q, float sub)
        {
            Quaternion result = { q.x - sub, q.y - sub, q.z - sub, q.w - sub };
            return result;
        }

        // Returns identity quaternion
        inline Quaternion QuaternionIdentity(void)
        {
            Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };
            return result;
        }

        // Computes the length of a quaternion
        inline float QuaternionLength(Quaternion q)
        {
            float result = (float)sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
            return result;
        }

        // Normalize provided quaternion
        inline Quaternion QuaternionNormalize(Quaternion q)
        {
            Quaternion result = { 0 };

            float length, ilength;
            length = QuaternionLength(q);
            if (length == 0.0f) length = 1.0f;
            ilength = 1.0f / length;

            result.x = q.x * ilength;
            result.y = q.y * ilength;
            result.z = q.z * ilength;
            result.w = q.w * ilength;

            return result;
        }

        // Invert provided quaternion
        inline Quaternion QuaternionInvert(Quaternion q)
        {
            Quaternion result = q;
            float length = QuaternionLength(q);
            float lengthSq = length * length;

            if (lengthSq != 0.0)
            {
                float i = 1.0f / lengthSq;

                result.x *= -i;
                result.y *= -i;
                result.z *= -i;
                result.w *= i;
            }

            return result;
        }

        // Calculate two quaternion multiplication
        inline Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2)
        {
            Quaternion result = { 0 };

            float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
            float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;

            result.x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
            result.y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
            result.z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
            result.w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

            return result;
        }

        // Scale quaternion by float value
        inline Quaternion QuaternionScale(Quaternion q, float mul)
        {
            Quaternion result = { 0 };

            float qax = q.x, qay = q.y, qaz = q.z, qaw = q.w;

            result.x = qax * mul + qaw * mul + qay * mul - qaz * mul;
            result.y = qay * mul + qaw * mul + qaz * mul - qax * mul;
            result.z = qaz * mul + qaw * mul + qax * mul - qay * mul;
            result.w = qaw * mul - qax * mul - qay * mul - qaz * mul;

            return result;
        }

        // Divide two quaternions
        inline Quaternion QuaternionDivide(Quaternion q1, Quaternion q2)
        {
            Quaternion result = { q1.x / q2.x, q1.y / q2.y, q1.z / q2.z, q1.w / q2.w };
            return result;
        }

        // Calculate linear interpolation between two quaternions
        inline Quaternion QuaternionLerp(Quaternion q1, Quaternion q2, float amount)
        {
            Quaternion result = { 0 };

            result.x = q1.x + amount * (q2.x - q1.x);
            result.y = q1.y + amount * (q2.y - q1.y);
            result.z = q1.z + amount * (q2.z - q1.z);
            result.w = q1.w + amount * (q2.w - q1.w);

            return result;
        }

        // Calculate slerp-optimized interpolation between two quaternions
        inline Quaternion QuaternionNlerp(Quaternion q1, Quaternion q2, float amount)
        {
            Quaternion result = QuaternionLerp(q1, q2, amount);
            result = QuaternionNormalize(result);

            return result;
        }

        // Calculates spherical linear interpolation between two quaternions
        inline Quaternion QuaternionSlerp(Quaternion q1, Quaternion q2, float amount)
        {
            Quaternion result = { 0 };

            float cosHalfTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

            if (fabs(cosHalfTheta) >= 1.0f) result = q1;
            else if (cosHalfTheta > 0.95f) result = QuaternionNlerp(q1, q2, amount);
            else
            {
                float halfTheta = acosf(cosHalfTheta);
                float sinHalfTheta = sqrtf(1.0f - cosHalfTheta * cosHalfTheta);

                if (fabs(sinHalfTheta) < 0.001f)
                {
                    result.x = (q1.x * 0.5f + q2.x * 0.5f);
                    result.y = (q1.y * 0.5f + q2.y * 0.5f);
                    result.z = (q1.z * 0.5f + q2.z * 0.5f);
                    result.w = (q1.w * 0.5f + q2.w * 0.5f);
                }
                else
                {
                    float ratioA = sinf((1 - amount) * halfTheta) / sinHalfTheta;
                    float ratioB = sinf(amount * halfTheta) / sinHalfTheta;

                    result.x = (q1.x * ratioA + q2.x * ratioB);
                    result.y = (q1.y * ratioA + q2.y * ratioB);
                    result.z = (q1.z * ratioA + q2.z * ratioB);
                    result.w = (q1.w * ratioA + q2.w * ratioB);
                }
            }

            return result;
        }

        // Calculate quaternion based on the rotation from one vector to another
        inline Quaternion QuaternionFromVector3ToVector3(vec3 from, vec3 to)
        {
            Quaternion result = { 0 };

            float cos2Theta = dot_product(from, to);
            vec3 cross = cross_product(from, to);

            result.x = cross.x;
            result.y = cross.y;
            result.z = cross.z;
            result.w = 1.0f + cos2Theta;     // NOTE: Added QuaternioIdentity()

            // Normalize to essentially nlerp the original and identity to 0.5
            result = QuaternionNormalize(result);

            // Above lines are equivalent to:
            //Quaternion result = QuaternionNlerp(q, QuaternionIdentity(), 0.5f);

            return result;
        }

        // Returns rotation quaternion for an angle and axis
        // NOTE: angle must be provided in radians
        inline Quaternion QuaternionFromAxisAngle(vec3 axis, float angle)
        {
            Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

            if (length(axis) != 0.0f)

                angle *= 0.5f;

            axis = normalized(axis);

            float sinres = sinf(angle);
            float cosres = cosf(angle);

            result.x = axis.x * sinres;
            result.y = axis.y * sinres;
            result.z = axis.z * sinres;
            result.w = cosres;

            result = QuaternionNormalize(result);

            return result;
        }

        // Returns the rotation angle and axis for a given quaternion
        inline void QuaternionToAxisAngle(Quaternion q, vec3* outAxis, float* outAngle)
        {
            if (fabs(q.w) > 1.0f) q = QuaternionNormalize(q);

            vec3 resAxis = { 0.0f, 0.0f, 0.0f };
            float resAngle = 2.0f * acosf(q.w);
            float den = sqrtf(1.0f - q.w * q.w);

            if (den > 0.0001f)
            {
                resAxis.x = q.x / den;
                resAxis.y = q.y / den;
                resAxis.z = q.z / den;
            }
            else
            {
                // This occurs when the angle is zero.
                // Not a problem: just set an arbitrary normalized axis.
                resAxis.x = 1.0f;
            }

            *outAxis = resAxis;
            *outAngle = resAngle;
        }

        // Returns the quaternion equivalent to Euler angles
        inline Quaternion QuaternionFromEuler(float roll, float pitch, float yaw)
        {
            Quaternion q = { 0 };

            float x0 = cosf(roll * 0.5f);
            float x1 = sinf(roll * 0.5f);
            float y0 = cosf(pitch * 0.5f);
            float y1 = sinf(pitch * 0.5f);
            float z0 = cosf(yaw * 0.5f);
            float z1 = sinf(yaw * 0.5f);

            q.x = x1 * y0 * z0 - x0 * y1 * z1;
            q.y = x0 * y1 * z0 + x1 * y0 * z1;
            q.z = x0 * y0 * z1 - x1 * y1 * z0;
            q.w = x0 * y0 * z0 + x1 * y1 * z1;

            return q;
        }
        inline Quaternion QuaternionFromEuler(vec3 rotation)
        {
            return QuaternionFromEuler(rotation.x, rotation.y, rotation.z);
        }

        // Return the Euler angles equivalent to quaternion (roll, pitch, yaw)
        // NOTE: Angles are returned in a Vector3 struct in degrees
        inline vec3 QuaternionToEuler(Quaternion q)
        {
            vec3 result = { 0 };

            // roll (x-axis rotation)
            float x0 = 2.0f * (q.w * q.x + q.y * q.z);
            float x1 = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
            result.x = atan2f(x0, x1) * RAD2DEG;

            // pitch (y-axis rotation)
            float y0 = 2.0f * (q.w * q.y - q.z * q.x);
            y0 = y0 > 1.0f ? 1.0f : y0;
            y0 = y0 < -1.0f ? -1.0f : y0;
            result.y = asinf(y0) * RAD2DEG;

            // yaw (z-axis rotation)
            float z0 = 2.0f * (q.w * q.z + q.x * q.y);
            float z1 = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
            result.z = atan2f(z0, z1) * RAD2DEG;

            return result;
        }

        // Transform a quaternion given a transformation matrix
        inline Quaternion QuaternionTransform(Quaternion q, mat4 mat)
        {
            Quaternion result = { 0 };

            /*
            result.x = mat.m0 * q.x + mat.m4 * q.y + mat.m8 * q.z + mat.m12 * q.w;
            result.y = mat.m1 * q.x + mat.m5 * q.y + mat.m9 * q.z + mat.m13 * q.w;
            result.z = mat.m2 * q.x + mat.m6 * q.y + mat.m10 * q.z + mat.m14 * q.w;
            result.w = mat.m3 * q.x + mat.m7 * q.y + mat.m11 * q.z + mat.m15 * q.w;
            */
            return result;
        }


        inline vec3 Vector3RotateByQuaternion(vec3 v, Quaternion q)
        {
            vec3 result = { 0 };

            result.x = v.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + v.y * (2 * q.x * q.y - 2 * q.w * q.z) + v.z * (2 * q.x * q.z + 2 * q.w * q.y);
            result.y = v.x * (2 * q.w * q.z + 2 * q.x * q.y) + v.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + v.z * (-2 * q.w * q.x + 2 * q.y * q.z);
            result.z = v.x * (-2 * q.w * q.y + 2 * q.x * q.z) + v.y * (2 * q.w * q.x + 2 * q.y * q.z) + v.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);

            return result;
        }

    }
}

