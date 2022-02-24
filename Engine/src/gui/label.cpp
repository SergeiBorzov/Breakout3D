#include "../third_party/stb_truetype.h"

#include "../renderer/renderer.h"
#include "../renderer/sprite.h"
#include "../window/fly_window.h"
#include "label.h"


namespace Fly {
namespace UI {

    f32 Label::CalcTextWidth(const Font* font, const char* text, i32 startIndex, i32 size) {
        f32 width = 0.0f;
        f32 scale = stbtt_ScaleForPixelHeight(font->GetFontInfo(), font->GetSize());
        for (int i = startIndex; i < startIndex + size; i++) {
            i32 advanceWidth, leftSideBearing;
            stbtt_GetCodepointHMetrics(font->GetFontInfo(), (i32)text[i], &advanceWidth, &leftSideBearing);
            width += scale*advanceWidth;
        }
        return width;
    }

    f32 Label::CalcTextHeight(const Font* font) {
        i32 ascent, descent, lineGap;
        stbtt_GetFontVMetrics(font->GetFontInfo(), &ascent, &descent, &lineGap);
        f32 scale = stbtt_ScaleForPixelHeight(font->GetFontInfo(), font->GetSize());
        return scale*ascent;
    }

    void Label::Update(f32 deltaTime) {
        if (!entity.IsActive()) {
            return;
        }
        
        i32 width = g_Window->GetWidth();
        i32 height = g_Window->GetHeight();

        // Determine label size
        Math::Vec2 labelSize;
        if (units == Units::Percents) {
            labelSize = Math::Vec2((f32)width, (f32)height)*size;
        }
        else {
            labelSize = size;
        }
        
        f32 textWidth = CalcTextWidth(m_Font, text.c_str(), 0, (i32)text.length());
        f32 textHeight = CalcTextHeight(m_Font);

        // Determine label position
        Math::Vec2 labelPosition;
        if (units == Units::Percents) {
            labelPosition = Math::Vec2((f32)width, (f32)height)*Math::Vec2(position);
        }
        else {
            labelPosition = Math::Vec2(position);
        }

        f32 x, y;
        switch (horizontalAlignment) {
            case Alignment::Left: {
                x = labelPosition.x - labelSize.x*0.5f;
                y = labelPosition.y + textHeight*0.5f;
                break;
            }
            case Alignment::Center: {
                x = labelPosition.x - textWidth*0.5f;
                y = labelPosition.y + textHeight*0.5f;
                break;
            }
            case Alignment::Right: {
                x = labelPosition.x + labelSize.x*0.5f - textWidth;
                y = labelPosition.y + textHeight*0.5f;
                break;
            }
        }


        // Submit background
        if (hasBackground) {
            Sprite backgroundSprite;
            backgroundSprite.uv = {0.0f, 0.0f, 1.0f, 1.0f};
            backgroundSprite.albedoMap = nullptr;
            backgroundSprite.color = backgroundColor;
            backgroundSprite.position = Math::Vec3(labelPosition, position.z);
            backgroundSprite.roundness = roundness;
            backgroundSprite.size = labelSize;
            g_Renderer->SubmitSprite(backgroundSprite);
        }


        // Submit text
        for (int i = 0; i < text.length(); i++) {
            if (text[i] >= 32 && text[i] < 128) {
                stbtt_aligned_quad q;
                
                stbtt_GetBakedQuad((const stbtt_bakedchar*)m_Font->GetBakedCharData(), 
                                    (i32)m_Font->GetBitmap()->GetWidth(), 
                                    (i32)m_Font->GetBitmap()->GetHeight(), 
                                    text[i] - 32, &x, &y, &q, 1); //1=opengl & d3d10+,0=d3d9

                Sprite sprite;
                sprite.uv = {q.s0, q.t1, q.s1, q.t0};
                sprite.albedoMap = m_Font->GetBitmap();
                sprite.color = textColor;
                sprite.position = Math::Vec3(0.5f*(q.x1 + q.x0), 0.5f*(q.y1 + q.y0), position.z);
                sprite.roundness = 0.0f;
                sprite.size = Math::Vec2(q.x1 - q.x0, q.y1 - q.y0);

                if (sprite.size.x > 0.0f && sprite.size.y > 0.0f) {
                    g_Renderer->SubmitSprite(sprite);
                }
            }
        }
    }
}
}