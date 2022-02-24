#define _USE_MATH_DEFINES
#include <math.h>

#include "general.h"
#include "quat.h"

namespace Math {
    Vec3 Quat::operator*(const Vec3& rhs) const {
        Vec3 q = {x, y, z};
        Vec3 cross1 = Cross(q, rhs);
        Vec3 cross2 = Cross(q, cross1);

        return rhs + 2.0f*(cross1*w + cross2);
    }

    Quat Quat::Euler(f32 x, f32 y, f32 z) {
        x = Radians(0.5f*x);
        y = Radians(0.5f*y);
        z = Radians(0.5f*z);

        f32 sinX = sinf(x);
        f32 cosX = cosf(x);
        f32 sinY = sinf(y);
        f32 cosY = cosf(y);
        f32 sinZ = sinf(z);
        f32 cosZ = cosf(z);

        f32 w = cosY*cosX*cosZ + sinY*sinX*sinZ;
        x = cosY*sinX*cosZ + sinY*cosX*sinZ;
        y = sinY*cosX*cosZ - cosY*sinX*sinZ;
        z = cosY*cosX*sinZ - sinY*sinX*cosZ;

        return Quat(w, x, y, z);
    }

    Quat Quat::Euler(const Vec3& angles) {
        f32 x = Radians(0.5f*angles.x);
        f32 y = Radians(0.5f*angles.y);
        f32 z = Radians(0.5f*angles.z);

        f32 sinX = sinf(x);
        f32 cosX = cosf(x);
        f32 sinY = sinf(y);
        f32 cosY = cosf(y);
        f32 sinZ = sinf(z);
        f32 cosZ = cosf(z);

        f32 w = cosY*cosX*cosZ + sinY*sinX*sinZ;
        x = cosY*sinX*cosZ + sinY*cosX*sinZ;
        y = sinY*cosX*cosZ - cosY*sinX*sinZ;
        z = cosY*cosX*sinZ - sinY*sinX*cosZ;

        return Quat(w, x, y, z);
    }

    Quat Quat::FromRotationMatrix(const Mat3& mat) {
        Quat q;
        f32 t = mat[0][0] + mat[1][1] + mat[2][2];

        if (t > 0.0f) {
            f32 invS = 0.5f/sqrtf(1.0f + t);

            q.x = (mat[2][1] - mat[1][2])*invS;
            q.y = (mat[0][2] - mat[2][0])*invS;
            q.z = (mat[1][0] - mat[0][1])*invS;
            q.w = 0.25f/invS;
        }   
        else {
            if (mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2]) {
                f32 invS = 0.5f/sqrtf(1.0f + mat[0][0] - mat[1][1] - mat[2][2]);

                q.x = 0.25f/invS;
                q.y = (mat[0][1] + mat[1][0])*invS;
                q.z = (mat[2][0] + mat[0][2])*invS;
                q.w = (mat[2][1] - mat[1][2])*invS;
            }
            else if (mat[1][1] > mat[2][2]) {
                f32 invS = 0.5f/sqrtf(1.0f + mat[1][1] - mat[0][0] - mat[2][2]);

                q.x = (mat[0][1] + mat[1][0])*invS;
                q.y = 0.25f/invS;
                q.z = (mat[1][2] + mat[2][1])*invS;
                q.w = (mat[0][2] - mat[2][0])*invS;
            }
            else {
                f32 invS = 0.5f/sqrtf(1.0f + mat[2][2] - mat[0][0] - mat[1][1]);

                q.x = (mat[0][2] + mat[2][0])*invS;
                q.y = (mat[1][2] + mat[2][1])*invS;
                q.z = 0.25f/invS;
                q.w = (mat[1][0] - mat[0][1])*invS;
            }
        }
        return q;
    }

    void Quat::ToEuler(const Quat& q, f32& x, f32& y, f32& z) {
        f32 t = 2.0f*(-q.y*q.z + q.w*q.x);

        if (t < -0.9999f) {
            x = -90.0f;
            y = 0.0f;
            z = Degrees(-atan2f(2.0f*(q.x*q.z - q.w*q.y), 1.0f - 2.0f*(q.y*q.y + q.z*q.z)));
        }
        else if (t > 0.9999f) {
            x = 90.0f;
            y = 0.0;
            z = Degrees(atan2f(2.0f*(q.x*q.z - q.w*q.y), 1.0f - 2.0f*(q.y*q.y + q.z*q.z)));
        }
        else {
            x = Degrees(asinf(t));
            y = Degrees(atan2f(2.0f*(q.x*q.z + q.w*q.y), 1.0f - 2.0f*(q.x*q.x + q.y*q.y)));
            z = Degrees(atan2f(2.0f*(q.x*q.y + q.w*q.z), 1.0f - 2.0f*(q.x*q.x + q.z*q.z)));
        }
    }

    void Quat::ToEuler(const Quat& q, Vec3& angles) {
        f32 t = 2.0f*(-q.y*q.z + q.w*q.x);

        if (t < -0.9999f) {
            angles.x = -90.0f;
            angles.y = 0.0f;
            angles.z = Degrees(-atan2f(2.0f*(q.x*q.z - q.w*q.y), 1.0f - 2.0f*(q.y*q.y + q.z*q.z)));
        }
        else if (t > 0.9999f) {
            angles.x = 90.0f;
            angles.y = 0.0;
            angles.z = Degrees(atan2f(2.0f*(q.x*q.z - q.w*q.y), 1.0f - 2.0f*(q.y*q.y + q.z*q.z)));
        }
        else {
            angles.x = Degrees(asinf(t));
            angles.y = Degrees(atan2f(2.0f*(q.x*q.z + q.w*q.y), 1.0f - 2.0f*(q.x*q.x + q.y*q.y)));
            angles.z = Degrees(atan2f(2.0f*(q.x*q.y + q.w*q.z), 1.0f - 2.0f*(q.x*q.x + q.z*q.z)));
        }
    }

    Mat3 Quat::RotationMatrix(const Quat& q) {
    return Mat3(Vec3(1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z,
                     2.0f*q.x*q.y - 2.0f*q.w*q.z,
                     2.0f*q.x*q.z + 2.0f*q.w*q.y),
                Vec3(2.0f*q.x*q.y + 2.0f*q.w*q.z,
                     1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z,
                     2.0f*q.y*q.z - 2.0f*q.w*q.x),
                Vec3(2.0f*q.x*q.z - 2.0f*q.w*q.y,
                     2.0f*q.y*q.z + 2.0f*q.w*q.x,
                     1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y)
    );
}
}
