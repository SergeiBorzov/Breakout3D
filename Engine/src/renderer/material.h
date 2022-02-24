#ifndef FLY_MATERIAL_H
#define FLY_MATERIAL_H

#include "shader.h"
#include "texture.h"

namespace Fly {
    class Material: public Resource {
        friend class Loader;
    public:
        Material(const Shader& shader): m_Shader(shader) {}

        virtual void SetProperties(const Math::Mat4& model) const {}
        virtual void SetProperties(f32 opaqueness, const Math::Mat4& model) const {}
    protected:
        const Shader& m_Shader;
    };

    class PBRMaterial: public Material {
    public:
        PBRMaterial();
        const Texture* albedoMap = nullptr;
        const Texture* normalMap = nullptr;
        const Texture* roughnessMap = nullptr;
        const Texture* metalnessMap = nullptr;

        Math::Vec3 albedo = Math::Vec3(1.0f);
        Math::Vec2 tiling = Math::Vec2(1.0f);
        f32 metalness = 0.5f;
        f32 roughness = 0.5f;
        f32 threshold = 1.0f;

        void SetProperties(f32 opaqueness, const Math::Mat4& model) const override;
    private:
        const Texture* noiseMap = nullptr;
    };
}

#endif /* End of FLY_MATERIAL */
