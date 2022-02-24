#ifndef MATH_RAY_H
#define MATH_RAY_H

#include "vec.h"

namespace Math {
    
    struct Ray {
        Math::Vec3 origin;
        Math::Vec3 direction;
    };

    bool RaySphereIntersection(const Ray& ray, const Math::Vec3& spherePos, f32 sphereRadius, f32& t1, f32& t2);
}


#endif /* End of MATH_RAY_H */