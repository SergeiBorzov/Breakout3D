#ifndef MATH_GENERAL_H
#define MATH_GENERAL_H

#include "../core/types.h"
#include "../math/vec.h"
#include "../math/mat.h"
#include "../math/quat.h"
#include "../math/ray.h"
namespace Math {
    const f32 PI = 3.14159265358979323846264338327950288f;
    const f32 EPS = (f32)1e-5;

    f32 Radians(f32 degrees);
    f32 Degrees(f32 radians);

    f32 Fract(f32 value);

    f32 Length(const Vec2& vec);
    f32 Length(const Vec3& vec);
    f32 Length(const Vec4& vec);
    f32 LengthSqr(const Vec2& vec);
    f32 LengthSqr(const Vec3& vec);
    f32 LengthSqr(const Vec4& vec);

    Vec2 Normalize(const Vec2& vec);
    Vec3 Normalize(const Vec3& vec);
    Vec4 Normalize(const Vec4& vec);
    Quat Normalize(const Quat& quat);

    i32 RandomInt(i32 minInclusive, i32 maxInclusive);
    
    inline f32 Dot(const Vec2& a, const Vec2& b) {
        return a.x*b.x + a.y*b.y;
    }
    
    inline f32 Dot(const Vec3& a, const Vec3& b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    
    inline f32 Dot(const Vec4& a, const Vec4& b) {
        return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    }

    inline Vec3 Cross(const Vec3& a, const Vec3& b) {
        f32 x = a.y*b.z - b.y*a.z;
        f32 y = -a.x*b.z + a.z*b.x;
        f32 z = a.x*b.y - b.x*a.y;
        
        return Vec3(x, y, z);
    }
    
    Mat4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
    Mat4 Perspective(f32 fovy, f32 aspect, f32 near, f32 far);
    Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

    inline Mat4 Translate(const Math::Vec3& translate) {
        Mat4 mat;
        mat[3] = Math::Vec4(translate, 1.0f);
        return mat;
    }

    inline Mat4 Scale(const Math::Vec3& scale) {
        Mat4 mat;
        mat[0][0] = scale.x;
        mat[1][1] = scale.y;
        mat[2][2] = scale.z;
        return mat;
    }

    inline Mat4 Rotation(const Quat& q) {
        f32 data[16];

        data[0] = 1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z;
        data[1] = 2.0f*q.x*q.y + 2.0f*q.w*q.z;
        data[2] = 2.0f*q.x*q.z - 2.0f*q.w*q.y;
        data[3] = 0.0f;
        data[4] = 2.0f*q.x*q.y - 2.0f*q.w*q.z;
        data[5] = 1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z;
        data[6] = 2.0f*q.y*q.z + 2.0f*q.w*q.x;
        data[7] = 0.0f;
        data[8] = 2.0f*q.x*q.z + 2.0f*q.w*q.y;
        data[9] = 2.0f*q.y*q.z - 2.0f*q.w*q.x;
        data[10] = 1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y;
        data[11] = 0.0f;
        data[12] = 0.0f;
        data[13] = 0.0f;
        data[14] = 0.0f;
        data[15] = 1.0f;

        return Mat4(data);    
    }

    inline f32 Sign(f32 value) {
        return (f32)((0.0f < value) - (value < 0.0f));
    }

    inline f32 Abs(f32 value) {
        return value * ((value > 0.0f) - (value < 0.0f));
    }

    i32 Max(i32 a, i32 b);
    u32 Max(u32 a, u32 b);
    f32 Max(f32 a, f32 b);
    Math::Vec2 Max(const Math::Vec2& a, const Math::Vec2& b);
    Math::Vec3 Max(const Math::Vec3& a, const Math::Vec3& b);
    Math::Vec4 Max(const Math::Vec4& a, const Math::Vec4& b);

    i32 Min(i32 a, i32 b);
    u32 Min(u32 a, u32 b);
    f32 Min(f32 a, f32 b);
    Math::Vec2 Min(const Math::Vec2& a, const Math::Vec2& b);
    Math::Vec3 Min(const Math::Vec3& a, const Math::Vec3& b);
    Math::Vec4 Min(const Math::Vec4& a, const Math::Vec4& b);

    f32 Sin(f32 radians);
    f32 Cos(f32 radians);
    f32 Sqrt(f32 value);

    inline f32 Clamp(f32 value, f32 min, f32 max) {
        return Min(Max(value, min), max);
    }

    inline i32 Clamp(i32 value, i32 min, i32 max) {
        return Min(Max(value, min), max);
    }

    inline u32 Clamp(u32 value, u32 min, u32 max) {
        return Min(Max(value, min), max);
    }

    inline f32 Lerp(f32 a, f32 b, f32 t) {
        return a*(1.0f - t) + b*t;        
    }

    inline Math::Vec3 Lerp(const Math::Vec3& a, const Math::Vec3& b, f32 t) {
        return a*(1.0f - t) + b*t;
    }
}
#endif /* End of MATH_GENERAL_H */
