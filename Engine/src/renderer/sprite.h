#ifndef FLY_SPRITE_H
#define FLY_SPRITE_H

#include "../core/types.h"
#include "../math/general.h"
#include "color.h"
#include "texture.h"

namespace Fly {
    struct Sprite {
        Math::Vec4 uv = {0.0f, 0.0f, 1.0f, 1.0f}; // (bot left corner, top right corner) in normalized coordinates [0..1]
        Math::Vec3 position = Math::Vec3(0.0f); // screen-space position (in pixels), z corresponds to layer
        const Texture* albedoMap = nullptr;
        Math::Vec2 size = Math::Vec2(50.0f); // size in pixels
        f32 roundness = 0.0f;
        Color color = {255, 255, 255, 255};
    };
}

#endif /* End of FLY_SPRITE */