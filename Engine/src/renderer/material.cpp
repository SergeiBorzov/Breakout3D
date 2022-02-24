#include "../core/log.h"
#include "../resource/resource_manager.h"
#include "../scene/camera_component.h"
#include "material.h"

namespace Fly {
    PBRMaterial::PBRMaterial():
        Material(*(g_ResourceManager->GetResource<Shader>("Assets/Shaders/pbr.glsl")))
    {
        noiseMap = g_ResourceManager->GetResource<Texture>("Assets/Textures/Noise/noise.png");
    }

    void PBRMaterial::SetProperties(f32 opaqueness, const Math::Mat4& model) const {
        Shader::Use(m_Shader);

        // Binding textures
        i32 bindings[] = {8, 9, 10, 11, 12, 13, 14, 15};
        m_Shader.SetUniform("u_ShadowMaps", bindings, 8);

        m_Shader.SetUniform("u_Model", model);
        m_Shader.SetUniform("u_Albedo", albedo);
        m_Shader.SetUniform("u_Tiling", tiling);
        m_Shader.SetUniform("u_CameraPos", CameraComponent::s_MainCamera->transform.GetPosition());
        m_Shader.SetUniform("u_Threshold", 1.0f - threshold);
        m_Shader.SetUniform("u_Opaqueness", opaqueness);

        if (albedoMap) {
            Texture::ActiveTexture(0);
            Texture::Bind(*albedoMap);
        }

        if (normalMap) {
            Texture::ActiveTexture(1);
            Texture::Bind(*normalMap);
        }

        if (roughnessMap) {
            Texture::ActiveTexture(2);
            Texture::Bind(*roughnessMap);
        }

        if (metalnessMap) {
            Texture::ActiveTexture(3);
            Texture::Bind(*metalnessMap);
        }

        Texture::ActiveTexture(4);
        Texture::Bind(*noiseMap);
    }
}
