#ifndef FLY_BUTTON_H
#define FLY_BUTTON_H

#include <functional>
#include "label.h"

namespace Fly {
namespace UI {

    class Button: public Label {
    public:
        Button(Entity& entity);
        void Update(f32 deltaTime) override;

        std::function<void(void)> onMouseHoverStart;
        std::function<void(void)> onMouseHover;
        std::function<void(void)> onMouseHoverEnd;
        std::function<void(void)> onMouseClick;
    private:
        bool wasHoveredLastFrame = false;
        bool isHoldingClick = false;
    };
}
}

#endif /* End of FLY_BUTTON_H */