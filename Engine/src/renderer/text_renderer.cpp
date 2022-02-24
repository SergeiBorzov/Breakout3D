#include "../third_party/stb_truetype.h"

#include "../core/fly_gl.h"
#include "../math/general.h"
#include "../window/fly_window.h"
#include "../resource/resource_manager.h"
#include "../gui/canvas.h"
#include "shader.h"
#include "font.h"
#include "text_renderer.h"


struct SpriteData {
    Math::Vec4 uv;
    Math::Vec3 position;
    Math::Vec2 size;
    f32 roundness;
    u32 color;
    u32 textureSlot;
};

namespace Fly {
    i32 TextRenderer::Init() {
        for (i32 i = 0; i < 16; i++) {
            m_TextureTable[i] = 0;
        }

        m_VertexBuffer.Create(nullptr, FLY_BATCH_SIZE*sizeof(SpriteData), BufferUsage::DynamicDraw);
        m_VertexArray.SetAttributes(m_VertexBuffer, 
                                    {
                                        {Attribute::Vec4, 0}, 
                                        {Attribute::Vec3, 1}, 
                                        {Attribute::Vec2, 2},
                                        {Attribute::f32, 3},
                                        {Attribute::Color, 4, true}, 
                                        {Attribute::u32, 5}, 
                                    });

        m_SpriteShader = g_ResourceManager->GetResource<Shader>("Assets/Shaders/gui.glsl");
        if (!m_SpriteShader) {
            return -1;
        }

        // Generating and storing white texture - placeholder for sprites with no texture
        GLCALL(glGenTextures(1, &m_WhiteTextureHandle));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_WhiteTextureHandle));
        u32 whitePixel = 0xffffffff;
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel));
        m_TextureTable[0] = m_WhiteTextureHandle;

        // Binding textures
        i32 bindings[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Shader::Use(*m_SpriteShader);
        m_SpriteShader->SetUniform("u_TextureTable", bindings, 16);
        return 0;
    }

    void TextRenderer::Draw() {
        GLCALL(glEnable(GL_DEPTH_TEST));
        // Canvas size = viewport size
        i32 viewport[4];
        GLCALL(glGetIntegerv(GL_VIEWPORT, viewport));
        Math::Mat4 projection = Math::Ortho(0.0f, (f32)viewport[2], (f32)viewport[3], 0.0f, -1.0f, 1.0f);

        Shader::Use(*m_SpriteShader);
        m_SpriteShader->SetUniform("u_Projection", projection);

        VertexArray::Bind(m_VertexArray);
        
        // Rebind texture table
        for (i32 i = 0; i < 16; i++) {
            Texture::ActiveTexture(i);
            GLCALL(glBindTexture(GL_TEXTURE_2D, m_TextureTable[i]));
        }

        u32 batchSize = 0;
        VertexBuffer::Bind(m_VertexBuffer);
        SpriteData* spriteBuffer = nullptr;
        GLCALL(spriteBuffer = (SpriteData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        for (u64 i = 0; i < m_Sprites.size(); i++) {
            if (batchSize == FLY_BATCH_SIZE) {
                // No more size in vertex buffer

                // Clear batch
                GLCALL(glUnmapBuffer(GL_ARRAY_BUFFER));
                GLCALL(glDrawArrays(GL_POINTS, 0, batchSize));

                GLCALL(spriteBuffer = (SpriteData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
                batchSize = 0;
            }
            u32 tableIndex = 0;
            u32 textureHandle = 0;

            if (m_Sprites[i].albedoMap != nullptr) {
                textureHandle = m_Sprites[i].albedoMap->GetHandle();
                for (i32 j = 1; j < 16; j++) {
                    if (m_TextureTable[j] == textureHandle) {
                        tableIndex = j;
                        break;
                    }
                    else if (m_TextureTable[j] == 0) {
                        tableIndex = j;
                        m_TextureTable[j] = textureHandle;
                        Texture::ActiveTexture(tableIndex);
                        GLCALL(glBindTexture(GL_TEXTURE_2D, textureHandle));
                        break;
                    }
                }

                if (tableIndex == 0) {
                    // No more textures can fit in batch!

                    // Clear batch
                    GLCALL(glUnmapBuffer(GL_ARRAY_BUFFER));
                    GLCALL(glDrawArrays(GL_POINTS, 0, batchSize));
                    
                    // Bind not fitting texture and reset texture table                    
                    m_TextureTable[1] = textureHandle;
                    Texture::ActiveTexture(1);
                    GLCALL(glBindTexture(GL_TEXTURE_2D, textureHandle));

                    for (i32 j = 2; j < 16; j++) {
                        m_TextureTable[j] = 0;
                    }

                    GLCALL(spriteBuffer = (SpriteData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

                    spriteBuffer->uv = m_Sprites[i].uv;
                    spriteBuffer->position = m_Sprites[i].position;
                    spriteBuffer->size = m_Sprites[i].size;
                    spriteBuffer->roundness = m_Sprites[i].roundness;
                    spriteBuffer->color = m_Sprites[i].color;
                    spriteBuffer->textureSlot = tableIndex;
                    spriteBuffer++;

                    batchSize = 1;

                    continue;
                }
            }

            spriteBuffer->uv = m_Sprites[i].uv;
            spriteBuffer->position = m_Sprites[i].position;
            spriteBuffer->size = m_Sprites[i].size;
            spriteBuffer->roundness = m_Sprites[i].roundness;
            spriteBuffer->color = m_Sprites[i].color;
            spriteBuffer->textureSlot = tableIndex;

            spriteBuffer++;
            batchSize++;
        }

        GLCALL(glUnmapBuffer(GL_ARRAY_BUFFER));
        GLCALL(glDrawArrays(GL_POINTS, 0, batchSize));

        m_Sprites.clear();
    }

    TextRenderer::~TextRenderer() {
        m_VertexArray.Destroy();
        m_VertexBuffer.Destroy();

        if (m_WhiteTextureHandle)
            GLCALL(glDeleteTextures(1, &m_WhiteTextureHandle));
    }
}