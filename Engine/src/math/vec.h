#ifndef MATH_VEC_H
#define MATH_VEC_H

#include <assert.h>
#include "../core/types.h"

namespace Math {
    struct Vec2;
    struct Vec3;
    struct Vec4;
    
    struct Vec2 {
        explicit Vec2(f32 v = 0.0f) {
            x = v; y = v;
        }
        
        Vec2(f32 v_x, f32 v_y) {
            x = v_x; y = v_y;
        }

        explicit Vec2(const Vec3& vec);
        explicit Vec2(const Vec4& vec);

        inline Vec2 operator-() const {
            return Vec2(-x, -y);
        }

        inline Vec2 operator+() const {
            return Vec2(x, y);
        }

        inline Vec2 operator+(const Vec2& rhs) const {
            return Vec2(x + rhs.x, y + rhs.y);
        }

        inline Vec2 operator-(const Vec2& rhs) const {
            return Vec2(x - rhs.x, y - rhs.y);
        }

        inline Vec2 operator*(const Vec2& rhs) const {
            return Vec2(x*rhs.x, y*rhs.y);
        }

        inline Vec2 operator/(const Vec2& rhs) const {
            return Vec2(x/rhs.x, y/rhs.y);
        }

        inline Vec2& operator+=(const Vec2& rhs) {
            x += rhs.x; y += rhs.y;
            return *this;
        }

        inline Vec2& operator-=(const Vec2& rhs) {
            x -= rhs.x; y -= rhs.y;
            return *this;
        }

        inline Vec2& operator*=(const Vec2& rhs) {
            x *= rhs.x; y *= rhs.y;
            return *this;
        }

        inline Vec2& operator/=(const Vec2& rhs) {
            x /= rhs.x; y /= rhs.y;
            return *this;
        }

        inline f32& operator[](i32 i) {
            assert(i >= 0 && i <= 1);
            return *(&x + i);
        }

        inline const f32& operator[](i32 i) const {
            assert(i >= 0 && i <= 1);
            return *(&x + i);
        }
        
        f32 x;
        f32 y;
    };

    inline Math::Vec2 operator+(f32 lhs, const Math::Vec2& rhs) {
        return Math::Vec2(lhs + rhs.x, lhs + rhs.y);
    }

    inline Math::Vec2 operator+(const Math::Vec2& lhs, f32 rhs) {
        return Math::Vec2(lhs.x + rhs, lhs.y + rhs);
    }

    inline Math::Vec2 operator-(const Math::Vec2& lhs, f32 rhs) {
        return Math::Vec2(lhs.x - rhs, lhs.y - rhs);
    }

    inline Math::Vec2 operator*(f32 lhs, const Math::Vec2& rhs) {
        return Math::Vec2(lhs*rhs.x, lhs*rhs.y);
    }

    inline Math::Vec2 operator*(const Math::Vec2& lhs, f32 rhs) {
        return Math::Vec2(lhs.x*rhs, lhs.y*rhs);
    }

    inline Math::Vec2 operator/(const Math::Vec2& lhs, f32 rhs) {
        return Math::Vec2(lhs.x/rhs, lhs.y/rhs);
    }

    struct Vec3 {
        explicit Vec3(f32 v = 0.0f) {
            x = v; y = v; z = v;
        }
        
        Vec3(f32 v_x, f32 v_y, f32 v_z) {
            x = v_x; y = v_y; z = v_z;
        }

        Vec3(const Vec2& vec, f32 v) {
            x = vec.x; y = vec.y; z = v;
        }

        Vec3(f32 v, const Vec2& vec) {
            x = v; y = vec.x; z = vec.y;
        }

        explicit Vec3(const Vec4& vec);

        inline Vec3 operator-() const {
            return Vec3(-x, -y, -z);
        }

        inline Vec3 operator+() const {
            return Vec3(x, y, z);
        }
        
        inline Vec3 operator+(const Vec3& rhs) const {
            return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        inline Vec3 operator-(const Vec3& rhs) const {
            return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        inline Vec3 operator*(const Vec3& rhs) const {
            return Vec3(x*rhs.x, y*rhs.y, z*rhs.z);
        }

        inline Vec3 operator/(const Vec3& rhs) const {
            return Vec3(x/rhs.x, y/rhs.y, z/rhs.z);
        }

        inline Vec3& operator+=(const Vec3& rhs) {
            x += rhs.x; y += rhs.y; z += rhs.z;
            return *this;
        }

        inline Vec3& operator-=(const Vec3& rhs) {
            x -= rhs.x; y -= rhs.y; z -= rhs.z;
            return *this;
        }

        inline Vec3& operator*=(const Vec3& rhs) {
            x *= rhs.x; y *= rhs.y; z *= rhs.z;
            return *this;
        }

        inline Vec3& operator/=(const Vec3& rhs) {
            x /= rhs.x; y /= rhs.y; z /= rhs.z;
            return *this;
        }

        inline f32& operator[](i32 i) {
            assert(i >= 0 && i <= 2);
            return *(&x + i);
        }

        inline const f32& operator[](i32 i) const {
            assert(i >= 0 && i <= 2);
            return *(&x + i);
        }
        
        f32 x;
        f32 y;
        f32 z;
    };

    inline Math::Vec3 operator+(f32 lhs, const Math::Vec3& rhs) {
        return Math::Vec3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
    }

    inline Math::Vec3 operator+(const Math::Vec3& lhs, f32 rhs) {
        return Math::Vec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
    }

    inline Math::Vec3 operator-(const Math::Vec3& lhs, f32 rhs) {
        return Math::Vec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
    }

    inline Math::Vec3 operator*(f32 lhs, const Math::Vec3& rhs) {
        return Math::Vec3(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
    }

    inline Math::Vec3 operator*(const Math::Vec3& lhs, f32 rhs) {
        return Math::Vec3(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
    }

    inline Math::Vec3 operator/(const Math::Vec3& lhs, f32 rhs) {
        return Math::Vec3(lhs.x/rhs, lhs.y/rhs, lhs.z/rhs);
    }
    
    struct Vec4 {
        explicit Vec4(f32 v = 0.0f) {
            x = v; y = v; z = v; w = v;
        }
        
        Vec4(f32 v_x, f32 v_y, f32 v_z, f32 v_w) {
            x = v_x; y = v_y; z = v_z; w = v_w;
        }

        Vec4(const Vec2& vec, f32 v_z, f32 v_w) {
            x = vec.x; y = vec.y; z = v_z; w = v_w;
        }

        Vec4(f32 v_x, const Vec2& vec, f32 v_w) {
            x = v_x; y = vec.x; z = vec.y; w = v_w;
        }

        Vec4(f32 v_x, f32 v_y, const Vec2& vec) {
            x = v_x; y = v_y; z = vec.x; w = vec.y;
        }

        Vec4(const Vec2& vec1, const Vec2& vec2) {
            x = vec1.x; y = vec1.y; z = vec2.x; w = vec2.y;
        }

        Vec4(const Vec3& vec, f32 v_w) {
            x = vec.x; y = vec.y; z = vec.z; w = v_w;
        }

        Vec4(f32 v_x, const Vec3& vec) {
            x = v_x; y = vec.x; z = vec.y; w = vec.z;
        }

        inline Vec4 operator-() const {
            return Vec4(-x, -y, -z, -w);
        }

        inline Vec4 operator+() const {
            return Vec4(x, y, z, w);
        }

        inline Vec4 operator+(const Vec4& rhs) const {
            return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        inline Vec4 operator-(const Vec4& rhs) const {
            return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        inline Vec4 operator*(const Vec4& rhs) const {
            return Vec4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
        }

        inline Vec4 operator/(const Vec4& rhs) const {
            return Vec4(x/rhs.x, y/rhs.y, z/rhs.z, w/rhs.w);
        }

        inline Vec4& operator+=(const Vec4& rhs) {
            x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
            return *this;
        }

        inline Vec4& operator-=(const Vec4& rhs) {
            x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
            return *this;
        }

        inline Vec4& operator*=(const Vec4& rhs) {
            x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
            return *this;
        }

        inline Vec4& operator/=(const Vec4& rhs) {
            x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
            return *this;
        }

        inline f32& operator[](i32 i) {
            assert(i >= 0 && i <= 3);
            return *(&x + i);
        }

        inline const f32& operator[](i32 i) const {
            assert(i >= 0 && i <= 3);
            return *(&x + i);
        }
        
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    inline Math::Vec4 operator+(f32 lhs, const Math::Vec4& rhs) {
        return Math::Vec4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
    }

    inline Math::Vec4 operator+(const Math::Vec4& lhs, f32 rhs) {
        return Math::Vec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
    }

    inline Math::Vec4 operator-(const Math::Vec4& lhs, f32 rhs) {
        return Math::Vec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
    }

    inline Math::Vec4 operator*(f32 lhs, const Math::Vec4& rhs) {
        return Math::Vec4(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z, lhs*rhs.w);
    }

    inline Math::Vec4 operator*(const Math::Vec4& lhs, f32 rhs) {
        return Math::Vec4(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs, lhs.w*rhs);
    }

    inline Math::Vec4 operator/(const Math::Vec4& lhs, f32 rhs) {
        return Math::Vec4(lhs.x/rhs, lhs.y/rhs, lhs.z/rhs, lhs.w/rhs);
    }
  
}
#endif /* End of MATH_VEC_H */
