#ifndef FLY_TEXT_RENDERER_H
#define FLY_TEXT_RENDERER_H

#include <vector>

#include "../core/types.h"
#include "sprite.h"
#include "vertex_array.h"

#define FLY_BATCH_SIZE 4096

namespace Fly {
    class Shader;
    class Font;

    class TextRenderer {
    public:
        inline void Submit(const Sprite& sprite) { m_Sprites.push_back(sprite); }
        i32 Init();
        void Draw();
        ~TextRenderer();
    private:
        u32 m_TextureTable[16]; // GL >= 3.3 guarantees at least 16 textures in fragment shader
        std::vector<Sprite> m_Sprites;
        VertexBuffer m_VertexBuffer;
        VertexArray m_VertexArray;
        Shader* m_SpriteShader;
        u32 m_WhiteTextureHandle = 0;
    };
}

#endif /* End of FLY_BATCH_RENDERER_H */