#ifndef FLY_SCENE_H
#define FLY_SCENE_H

#include <vector>

#include "entity.h"

namespace Fly {
    class Scene {
    public:
        Entity* CreateEntity(const std::string& name);
        void DestroyEntity(Entity* entity);
        void ClearScene();
        
        ~Scene();
    private:
        std::vector<Entity*> m_Entities;
    };
}
#endif /* End of FLY_SCENE_H */
