#ifndef CORNER_LIGHT_H
#define CORNER_LIGHT_H

#include "fly.h"

class CornerLight: public Fly::NativeScript {
public:
    CornerLight(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& intensity): 
        Fly::NativeScript(entity),
        m_Position(position), 
        m_Intensity(intensity) {}
    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;

    void StartBlinking();
    inline void SetIntensity(const Math::Vec3& intensity) { m_LightComponent->intensity = intensity; }

    virtual ~CornerLight() {}
private:
    Math::Vec3 NextIntensity();

    Math::Vec3 m_Position;
    Math::Vec3 m_Intensity;

    Fly::LightComponent* m_LightComponent;
    f32 m_CurrentTime = 0.0f;
    u32 m_NumBlinks = 0;
    bool m_Blinking = false;
    bool m_LightOn = true;
};

#endif /* End of CORNER_LIGHT_H */