#include "../core/fly_gl.h"
#include "../window/fly_window.h"
#include "../renderer/renderer.h"
#include "light_component.h"

namespace Fly {
    const u32 LightComponent::s_ComponentID = ComponentID<LightComponent>::value();

    LightComponent::LightComponent(Fly::Entity& entity): Component(entity) {
        g_Renderer->RegisterComponent(this);
    }

    Math::Mat4 LightComponent::GetLightMatrix() const {
        Math::Mat4 res;
        switch (type) {
            case Type::Directional: {
                res = Math::Ortho(-25.0f, 25.0f, -25.0f, 25.0f, 3.0f, 100.0f)*
                      Math::LookAt(transform.GetPosition(), transform.GetPosition() + transform.GetForward(), Math::Vec3(0.0f, 1.0f, 0.0f));
                break;
            }
            case Type::Point:
            case Type::Spot: {
                res =  Math::Perspective(m_OuterAngle, 1.0f, 3.0f, 100.0f)*
                       Math::LookAt(transform.GetPosition(), transform.GetPosition() + transform.GetForward(), Math::Vec3(0.0f, 1.0f, 0.0f));
                break;
            }
        }
        return res;
    }

    void LightComponent::SetGenerateShadows(bool value) {
        m_GenerateShadows = value;
        if (value) {
            if (!m_ShadowMapHandle) {
                GLCALL(glGenTextures(1, &m_ShadowMapHandle));
                GLCALL(glBindTexture(GL_TEXTURE_2D, m_ShadowMapHandle));
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ShadowMapWidth, 
                                    m_ShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // outside fragments are not in shadow!
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
                GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
            }
        }
    }

    LightComponent::~LightComponent() {
        if (m_ShadowMapHandle) {
            GLCALL(glDeleteTextures(1, &m_ShadowMapHandle));
        }
        g_Renderer->DeregisterComponent(this);
    }
}