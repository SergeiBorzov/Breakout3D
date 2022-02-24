#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#include "../core/platform.h"
#include "general.h"

namespace Math {
    i32 RandomInt(i32 minInclusive, i32 maxInclusive) {
        return (rand() % (maxInclusive - minInclusive + 1)) + minInclusive;
    }

    i32 Max(i32 a, i32 b) {
        return std::max(a, b);
    }
    u32 Max(u32 a, u32 b) {
        return std::max(a, b);
    }
    f32 Max(f32 a, f32 b) {
        return std::max(a, b);
    }
    Math::Vec2 Max(const Math::Vec2& a, const Math::Vec2& b) {
        return Math::Vec2(std::max(a.x, b.x), std::max(a.y, b.y));
    }
    Math::Vec3 Max(const Math::Vec3& a, const Math::Vec3& b) {
        return Math::Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }
    Math::Vec4 Max(const Math::Vec4& a, const Math::Vec4& b) {
        return Math::Vec4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
    }

    i32 Min(i32 a, i32 b) {
        return std::min(a, b);
    }
    u32 Min(u32 a, u32 b) {
        return std::min(a, b);
    }
    f32 Min(f32 a, f32 b) {
        return std::min(a, b);
    }
    Math::Vec2 Min(const Math::Vec2& a, const Math::Vec2& b) {
        return Math::Vec2(std::min(a.x, b.x), std::min(a.y, b.y));
    }
    Math::Vec3 Min(const Math::Vec3& a, const Math::Vec3& b) {
        return Math::Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }
    Math::Vec4 Min(const Math::Vec4& a, const Math::Vec4& b) {
        return Math::Vec4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
    }



    f32 Sin(f32 radians) {
        return sinf(radians);
    }

    f32 Cos(f32 radians) {
        return cosf(radians);
    }

    f32 Radians(f32 degrees) {
        return (f32)(degrees*PI/180.0f);
    }

    f32 Degrees(f32 radians) {
        return (f32)(radians*180.0f/PI);
    }

    f32 Fract(f32 value) {
        f32 t;
        return modff(value, &t);
    }

    f32 Sqrt(f32 value) {
        return sqrtf(value);
    }

    f32 Length(const Vec2& vec) {
        return sqrtf(vec.x*vec.x + vec.y*vec.y);
    }

    f32 Length(const Vec3& vec) {
        return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
    }

    f32 Length(const Vec4& vec) {
        return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
    }

    f32 LengthSqr(const Vec2& vec) {
        return vec.x*vec.x + vec.y*vec.y;
    }

    f32 LengthSqr(const Vec3& vec) {
        return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
    }

    f32 LengthSqr(const Vec4& vec) {
        return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w;
    }

    Vec2 Normalize(const Vec2& vec) {
        f32 length = Length(vec);
        if (length < EPS) {
            return Vec2(0.0f);
        }
        return vec/length;
    }
    
    Vec3 Normalize(const Vec3& vec) {
        f32 length = Length(vec);
        if (length < EPS) {
            return Vec3(0.0f);
        }
        return vec/length;
    }
    
    Vec4 Normalize(const Vec4& vec) {
        f32 length = Length(vec);
        if (length < EPS) {
            return Vec4(0.0f);
        }
        return vec/length;
    }

    Quat Normalize(const Quat& quat) {
        f32 length = quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z*quat.z;
        if (length < EPS) {
            return Quat(0.0f, 0.0f, 0.0f, 0.0f);
        }
        return Quat(quat.w/length, quat.x/length, quat.y/length, quat.z/length);
    }
    
    Mat4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
        f32 data[16] = {0.0f};
        data[0] = 2.0f/(right - left);
        data[5] = 2.0f/(top - bottom);
        data[10] = -2.0f/(far - near);
        data[12] = -(right + left)/(right - left);
        data[13] = -(top + bottom)/(top - bottom);
        data[14] = -(far + near)/(far - near);
        data[15] = 1.0f;

        return Mat4(data);
    }

    Mat4 Perspective(f32 fovy, f32 aspect, float near, float far) {
        f32 data[16] = {0.0f};
        data[0] = 1.0f/(aspect*tanf(Radians(fovy)*0.5f));
        data[5] = 1.0f/(tanf(Radians(fovy)*0.5f));
        data[10] = -(far + near)/(far - near);
        data[11] = -1.0f;
        data[14] = -(2.0f*far*near)/(far - near);
            
        return Mat4(data);
    }

    Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Mat4 m;

        Vec3 z = Normalize(eye - center);
        Vec3 y = up;
        Vec3 x = Normalize(Cross(y, z));
        y = Normalize(Cross(z, x));

        m[0][0] = x.x;
        m[1][0] = x.y;
        m[2][0] = x.z;
        m[3][0] = -Dot(x, eye);
        m[0][1] = y.x;
        m[1][1] = y.y;
        m[2][1] = y.z;
        m[3][1] = -Dot(y, eye);
        m[0][2] = z.x;
        m[1][2] = z.y;
        m[2][2] = z.z;
        m[3][2] = -Dot(z, eye);
        m[0][3] = 0;
        m[1][3] = 0;
        m[2][3] = 0;
        m[3][3] = 1.0f;

        return m;
    }
}
