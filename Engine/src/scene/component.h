#ifndef FLY_COMPONENT_H
#define FLY_COMPONENT_H

#include "../core/platform.h"
#include "../core/error.h"
#include "../core/types.h"

#define FLY_MAX_COMPONENTS 32

namespace Fly {
    
    namespace Internal {
        struct ComponentID {
            static u32 next() {
                static u32 value;
                return ++value;
            }
        };        
    }

    template <typename T>
    struct ComponentID {
        static const u32 value() {
            static const u32 value = Internal::ComponentID::next();
            return value;
        }
    };

    class Entity;
    class Transform;
    class Scene;

    class Component {
    public:
        Component(Entity& entity);
        virtual ~Component() {}
        FLY_API static const u32 s_ComponentID = 0;
    public:
        Entity& entity;
        Transform& transform;
        Scene& scene;
    };
}

#endif /* End of FLY_COMPONENT_H */
