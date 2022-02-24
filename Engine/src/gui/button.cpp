#include "../third_party/stb_truetype.h"

#include "../renderer/renderer.h"
#include "../renderer/sprite.h"
#include "../window/fly_window.h"
#include "../window/input.h"
#include "button.h"

namespace Fly {
namespace UI {

    static void EmptyFunction() {}

    Button::Button(Entity& entity): Label(entity) {
        onMouseHoverStart = std::bind(EmptyFunction);
        onMouseHover = std::bind(EmptyFunction);
        onMouseHoverEnd = std::bind(EmptyFunction);
        onMouseClick = std::bind(EmptyFunction);
    }

    void Button::Update(f32 deltaTime) {
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
        //labelSize = Math::Max(labelSize, {textWidth, textHeight});// + Math::Vec2(10.0f, 10.0f);

        // Determine label position
        Math::Vec2 labelPosition;
        if (units == Units::Percents) {
            labelPosition = Math::Vec2((f32)width, (f32)height)*Math::Vec2(position);
        }
        else {
            labelPosition = Math::Vec2(position);
        }

        // Check mouse cursor and call callback functions
        Math::Vec2 mousePos;
        Input::GetMousePosition(&mousePos.x, &mousePos.y);

        Math::Vec2 topLeft = labelPosition - labelSize*0.5f;
        Math::Vec2 botRight = labelPosition + labelSize*0.5f;
        if (mousePos.x > topLeft.x && mousePos.x < botRight.x &&
            mousePos.y > topLeft.y && mousePos.y < botRight.y) {
            if (!wasHoveredLastFrame) {
                if (onMouseHoverStart) {
                    onMouseHoverStart();
                }
                wasHoveredLastFrame = true;
            }

            if (Input::GetMouseButtonDown(MouseButton::Left)) {
                isHoldingClick = true;
            }

            if (Input::GetMouseButtonUp(MouseButton::Left) && isHoldingClick) {
                onMouseClick();
                isHoldingClick = false;
            }
            
            if (onMouseHover) {
                onMouseHover();
            }
        }
        else {
            if (wasHoveredLastFrame) {
                onMouseHoverEnd();
                wasHoveredLastFrame = false;
            }

            if (Input::GetMouseButtonUp(MouseButton::Left) && isHoldingClick) {
                isHoldingClick = false;
            }
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