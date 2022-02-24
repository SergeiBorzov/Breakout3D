#ifndef FLY_CANVAS_H
#define FLY_CANVAS_H

#include <vector>

#include "../core/platform.h"
#include "widget.h"

namespace Fly {
namespace UI {
    class Canvas {
    public:
        void AddWidget(Widget* widget);
        void RemoveWidget(Widget* widget);
        inline const std::vector<Widget*>& GetWidgets() const { return m_Widgets; }

        void Update(f32 deltaTime);

        ~Canvas();
    private:
        std::vector<Widget*> m_Widgets;
    };

    FLY_API extern Canvas* g_Canvas;
}
}


#endif /* End of FLY_CANVAS_H */