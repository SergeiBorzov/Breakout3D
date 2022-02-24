#ifndef FLY_RENDER_COMPONENT_H
#define FLY_RENDER_COMPONENT_H

#include "../renderer/mesh.h"
#include "../renderer/material.h"
#include "entity.h"

namespace Fly {
    class RenderComponent: public Component {
        friend class Renderer;
    public:
        FLY_API static const u32 s_ComponentID;
        
        RenderComponent(Fly::Entity& entity);
        RenderComponent(Fly::Entity& entity, const Mesh* mesh, const Material* material);

        void Draw() const;
        virtual ~RenderComponent();

        inline bool IsTransparent() const { return isTransparent; }
        void SetTransparency(bool value);        

        const Mesh* mesh;
        std::vector<const Material*> materials;
        f32 opaqueness = 1.0f;
    private:
        bool isTransparent = false;
        void DrawInternal(const Shader& shader, const Math::Mat4& projectionView) const;
    };
}
#endif /* End of FLY_RENDER_COMPONENT_H */
