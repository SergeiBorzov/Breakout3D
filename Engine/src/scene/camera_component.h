#ifndef FLY_CAMERA_COMPONENT_H
#define FLY_CAMERA_COMPONENT_H

#include "../math/general.h"
#include "../scene/entity.h"

namespace Fly {
    class CameraComponent: public Component {
    public:
        CameraComponent(Entity& entity): Component(entity) {}
        
        inline void SetProjection(const Math::Mat4& projection) { m_Projection = projection; }
        inline const Math::Mat4& GetProjection() const { return m_Projection; }
        Math::Mat4 GetView() const;
        
        virtual ~CameraComponent() {}
        FLY_API static const u32 s_ComponentID;
        FLY_API static CameraComponent* s_MainCamera;
    private:
        Math::Mat4 m_Projection;
    };
}
#endif /* End of FLY_CAMERA_COMPONENT_H */
