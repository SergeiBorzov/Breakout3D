#ifndef MATH_QUAT_H
#define MATH_QUAT_H

#include "mat.h"

namespace Math {
    struct Quat {
        Quat(f32 v_w = 1.0f, f32 v_x = 0.0f, f32 v_y = 0.0f, f32 v_z = 0.0f):
            w(v_w), x(v_x), y(v_y), z(v_z)
        {}

        inline Quat operator+(const Quat& rhs) const {
            return {w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z};
        }

        inline Quat operator-(const Quat& rhs) const {
            return {w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z};
        }
        
        inline Quat operator/(f32 rhs) {
            return Quat(w/rhs, x/rhs, y/rhs, z/rhs);
        }

        inline Quat operator*(f32 rhs) {
            return Quat(w*rhs, x*rhs, z*rhs, z*rhs);
        }
        
        inline Quat operator*(const Quat& rhs) const {
            return Quat(w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z,
                        w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
                        w*rhs.y + y*rhs.w + z*rhs.x - x*rhs.z,
                        w*rhs.z + z*rhs.w + x*rhs.y - y*rhs.x);
        }

        Vec3 operator*(const Vec3& rhs) const;


        static Quat Euler(f32 x, f32 y, f32 z);
        static Quat Euler(const Vec3& angles);
        static Quat FromRotationMatrix(const Mat3& mat);
        
        inline static Quat FromAxes(const Math::Vec3& xAxis, const Math::Vec3& yAxis, const Math::Vec3& zAxis) {
            return FromRotationMatrix(Mat3(xAxis, yAxis, zAxis));
        }

        static void ToEuler(const Quat& quat, f32& x, f32& y, f32& z);
        static void ToEuler(const Quat& quat, Vec3& angles);

        static Mat3 RotationMatrix(const Quat& quat);

        f32 w;
        f32 x;
        f32 y;
        f32 z;
    };

    inline Quat Conjugate(const Quat& quat) {
        return Quat(quat.w, -quat.x, -quat.y, -quat.z);
    }

    inline Quat Inverse(const Quat& quat) {
        return Conjugate(quat)/(quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z*quat.z);
    }

}

#endif /* MATH_QUATERNION_H */
