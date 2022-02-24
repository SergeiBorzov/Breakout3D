#ifndef FLY_LIGHT_COMPONENT_H
#define FLY_LIGHT_COMPONENT_H

#include "entity.h"

namespace Fly {
    class LightComponent: public Component {
    public:
        enum Type {
            Directional = 0,
            Point = 1,
            Spot = 2
        };

        LightComponent(Fly::Entity& entity);
        virtual ~LightComponent();

        Type type = Type::Point;
        inline f32 GetOuterAngle() const { return m_OuterAngle; }
        inline f32 GetInnerAngle() const { return m_InnerAngle; }
        inline f32 GetOuterAngleCos() const { return m_OuterAngleCos; }
        inline f32 GetInnerAngleCos() const { return m_InnerAngleCos; }
        inline void SetOuterAngle(f32 angle) { m_OuterAngle = angle; m_OuterAngleCos = Math::Cos(Math::Radians(angle)*0.5f); }
        inline void SetInnerAngle(f32 angle) { m_InnerAngle = angle; m_InnerAngleCos = Math::Cos(Math::Radians(angle)*0.5f); }

        void SetGenerateShadows(bool value);
        inline bool GetGenerateShadows() const { return m_GenerateShadows; }
        inline u32 GetShadowMapHandle() const { return m_ShadowMapHandle; }
        inline i32 GetShadowMapWidth() const { return m_ShadowMapWidth; }
        inline i32 GetShadowMapHeight() const { return m_ShadowMapHeight; }

        Math::Mat4 GetLightMatrix() const;
       
        Math::Vec3 intensity = Math::Vec3(1.0f);

        FLY_API static const u32 s_ComponentID;
    private:
        f32 m_OuterAngle = 60.0f;
        f32 m_InnerAngle = 60.0f;
        f32 m_OuterAngleCos = 0.5f;
        f32 m_InnerAngleCos = 0.5f;
        u32 m_ShadowMapHandle = 0;
        i32 m_ShadowMapWidth = 2048;
        i32 m_ShadowMapHeight = 2048;
        bool m_GenerateShadows = false;
    };
}

#endif /* End of FLY_LIGHT_COMPONENT_H */