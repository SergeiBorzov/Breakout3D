#include "general.h"

namespace Math {
    bool RaySphereIntersection(const Ray& ray, const Math::Vec3& spherePos, f32 sphereRadius, f32& t1, f32& t2) {
        f32 a = Math::Dot(ray.direction, ray.direction);
        f32 b = 2.0f*Math::Dot(ray.direction, ray.origin - spherePos);
        f32 c = Math::Dot(ray.origin - spherePos, ray.origin - spherePos) - sphereRadius*sphereRadius;

        f32 d = b*b - 4.0f*a*c;

        if (d < 0.0f) {
            return false;
        }
        t1 = (-b - d)/2.0f*a;
        t2 = (-b + d)/2.0f*a;
        return true;
    }
}