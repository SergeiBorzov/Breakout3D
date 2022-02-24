#include "main_light.h"

using namespace Fly;

void MainLight::OnStart() {
    transform.SetPosition(m_Position);
    m_LightComponent = entity.AddComponent<LightComponent>();
    m_LightComponent->type = LightComponent::Type::Spot;
    m_LightComponent->transform.SetPosition(m_Position);
    m_LightComponent->transform.Rotate(-89.0f, 0.0f, 0.0f);
    m_LightComponent->SetOuterAngle(120.0f);
    m_LightComponent->SetInnerAngle(80.0f);
    m_LightComponent->intensity = m_Intensity;
    m_LightComponent->SetGenerateShadows(true);
}

void MainLight::StartBlinking() {
    m_Blinking = true;
    m_LightComponent->intensity = Math::Vec3(0.0f);
    m_LightOn = false;
}

Math::Vec3 MainLight::NextIntensity() {
    if (m_LightOn) {
        m_LightOn = false;
        return Math::Vec3(0.0f);
    }
    else {
        m_LightOn = true;
        return m_Intensity;
    }
}

void MainLight::OnUpdate(f32 deltaTime) {
    if (m_Blinking) {
        m_CurrentTime += deltaTime;

        if (m_CurrentTime > 1.5f) {
            if (Math::Fract(m_CurrentTime) > 0.3f*m_TimeMultiplier) {
                m_TimeMultiplier *= 0.7f;
                m_CurrentTime = 3.0f;
                m_LightComponent->intensity = NextIntensity();
                m_NumBlinks++;
            }

            if (m_NumBlinks == 20) {
                m_TimeMultiplier = 1.0f;
                m_NumBlinks = 0;
                m_Blinking = false;
                m_LightOn = true;
                m_CurrentTime = 0.0f;
                m_LightComponent->intensity = m_Intensity;
            }
        }


    }
}