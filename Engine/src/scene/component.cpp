#include "scene.h"

namespace Fly {
    Component::Component(Entity& entity): 
        entity(entity), 
        transform(entity.transform), 
        scene(entity.scene) 
    {}
}