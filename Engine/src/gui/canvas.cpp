#include "canvas.h"

namespace Fly {
namespace UI {
    Canvas* g_Canvas = nullptr;

    void Canvas::AddWidget(Widget* widget) {
        m_Widgets.push_back(widget);
    }

    void Canvas::RemoveWidget(Widget* widget) {
        auto it = std::find(m_Widgets.begin(), m_Widgets.end(), widget);

        if (it != m_Widgets.end()) {
            std::iter_swap(it, m_Widgets.end() - 1);
            m_Widgets.pop_back();
        }
    }

    void Canvas::Update(f32 deltaTime) {
        for (int i = 0; i < m_Widgets.size(); i++) {
            m_Widgets[i]->Update(deltaTime);
        }
    }

    Canvas::~Canvas() {
        for (u64 i = 0; i < m_Widgets.size(); i++) {
            delete m_Widgets[i];
        }
    }
}
}