#include "camera_component.h"

namespace Fly {
    const u32 CameraComponent::s_ComponentID = ComponentID<CameraComponent>::value();
    CameraComponent* CameraComponent::s_MainCamera = nullptr;
    
    Math::Mat4 CameraComponent::GetView() const {
        return LookAt(entity.transform.GetPosition(),
                      entity.transform.GetPosition() + entity.transform.GetForward(),
                      Math::Vec3(0.0f, 1.0f, 0.0f));
    }

    
}
