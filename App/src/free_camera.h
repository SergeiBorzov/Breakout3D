#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#include "fly.h"

class FreeCamera: public Fly::NativeScript {
public:
    FreeCamera(Fly::Entity& entity): Fly::NativeScript(entity) {}

    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;

    virtual ~FreeCamera() {}
private:
    void UpdatePosition(f32 deltaTime);
    void UpdateRotation();
    f32 m_Speed = 10.0f;
    f32 m_Sensitivity = 0.2f;

    bool m_FirstMouse = true;
    Math::Vec2 m_MousePosition;
    Math::Vec2 m_PrevMousePosition;
};

#endif /* End of FREE_CAMERA_H */