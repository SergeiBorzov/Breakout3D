#include "vec.h"

namespace Math {
    Vec2::Vec2(const Vec3& vec) {
        x = vec.x; y = vec.y;
    }

    Vec2::Vec2(const Vec4& vec) {
        x = vec.x; y = vec.y;
    }

    Vec3::Vec3(const Vec4& vec) {
        x = vec.x; y = vec.y; z = vec.z;
    }
}
