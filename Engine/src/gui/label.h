#ifndef FLY_LABEL_H
#define FLY_LABEL_H

#include <string>

#include "widget.h"
#include "../renderer/font.h"

namespace Fly {
namespace UI {
    enum class Alignment {
        Left, Center, Right
    };

    class Label: public Widget {
    public:
        Label(Entity& entity): Widget(entity) {}
        inline void SetFont(Font* font) { m_Font = font; }
        inline const Font* GetFont() const { return m_Font; }

        void Update(f32 deltaTime) override;

        std::string text = "New Label";
        
        Alignment horizontalAlignment = Alignment::Center;
        Color textColor = Color(255, 255, 255, 255);
    protected:
        static f32 CalcTextWidth(const Font* font, const char* text, i32 startIndex, i32 size);
        static f32 CalcTextHeight(const Font* font);
        Font* m_Font = nullptr;
    };
}
}

#endif /* End of FLLY_LABEL_H */