#ifndef FLY_WIDGET_H
#define FLY_WIDGET_H

#include "../renderer/color.h"
#include "../scene/entity.h"

namespace Fly {
namespace UI {
    enum class Units {
        Pixels,
        Percents
    };

    class Widget: public Component {
    protected:
        Widget(Entity& entity);
    public:
        Math::Vec3 position = Math::Vec3(0.0f);
        Math::Vec2 size;
        Units units = Units::Pixels;
        f32 roundness = 30.0f;
        bool hasBackground = true;
        Color backgroundColor = Color(0, 0, 139, 255);

        virtual void Update(f32 deltaTime) = 0;

        virtual ~Widget();

        FLY_API static const u32 s_ComponentID;
    };
}
}

#endif /* End of FLY_WIDGET_H */