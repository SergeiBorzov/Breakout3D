#include "widget.h"
#include "canvas.h"
namespace Fly {
namespace UI {
    const u32 Widget::s_ComponentID = ComponentID<Widget>::value();

    Widget::Widget(Fly::Entity& entity): Component(entity) {
        g_Canvas->AddWidget(this);
    }

    Widget::~Widget() {
        g_Canvas->RemoveWidget(this);
    }
}
}