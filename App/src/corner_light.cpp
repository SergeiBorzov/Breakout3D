#include "corner_light.h"

using namespace Fly;

void CornerLight::OnStart() {
    transform.SetPosition(m_Position);
    m_LightComponent = entity.AddComponent<LightComponent>();
    m_LightComponent->type = LightComponent::Type::Point;
    m_LightComponent->intensity = m_Intensity;
}

void CornerLight::StartBlinking() {
    m_Blinking = true;
    m_LightComponent->intensity = Math::Vec3(200.0f, 0.0f, 0.0f);
    //m_LightOn = false;
}

Math::Vec3 CornerLight::NextIntensity() {
    if (m_LightOn) {
        m_LightOn = false;
        return Math::Vec3(0.0f);
    }
    else {
        m_LightOn = true;
        return Math::Vec3(200.0f, 0.0f, 0.0f);
    }
}

void CornerLight::OnUpdate(f32 deltaTime) {
    if (m_Blinking) {
        m_CurrentTime += deltaTime;

        if (Math::Fract(m_CurrentTime) > 0.25f) {
            m_CurrentTime = 0.0f;
            m_LightComponent->intensity = NextIntensity();
            m_NumBlinks++;
        }

        if (m_NumBlinks == 6) {
            m_Blinking = false;
            m_NumBlinks = 0;
            m_LightOn = true;
            m_LightComponent->intensity = m_Intensity;
            m_CurrentTime = 0.0f;
        }
    }
}