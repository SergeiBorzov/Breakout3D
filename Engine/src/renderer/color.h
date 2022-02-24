#ifndef FLY_COLOR_H
#define FLY_COLOR_H

#include "../core/types.h"
#include "../math/general.h"

namespace Fly {
    struct Color {
        Color(f32 r, f32 g, f32 b, f32 a) {
            this->r = Math::Clamp((u32)(r*256.0f), 0u, 255u);
            this->g = Math::Clamp((u32)(g*256.0f), 0u, 255u);
            this->b = Math::Clamp((u32)(b*256.0f), 0u, 255u);
            this->a = Math::Clamp((u32)(a*256.0f), 0u, 255u);
        }

        Color (i32 r, i32 g, i32 b, i32 a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        operator u32() const { return (u32)a << 24 | (u32)b << 16 | (u32)g << 8 | (u32)r; }
        u8 r = 255;
        u8 g = 255;
        u8 b = 255;
        u8 a = 255;
    };
}

#endif /* End of FLY_COLOR_H */