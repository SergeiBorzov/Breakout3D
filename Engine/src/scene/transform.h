#ifndef FLY_TRANSFORM_H
#define FLY_TRANSFORM_H

#include "../math/general.h"

namespace Fly {
    class Entity;
    
    class Transform {
        friend class Entity;
    public:
        Transform(Entity& entity): m_Entity(entity) {}
        
        inline Math::Vec3 GetPosition() const { return m_Position; }
        inline Math::Vec3 GetScale() const { return m_Scale; }
        inline Math::Quat GetRotation() const { return m_Rotation; }
        inline Math::Vec3 GetLocalPosition() const { return m_LocalPosition; }
        inline Math::Vec3 GetLocalScale() const { return m_LocalScale; }
        inline Math::Quat GetLocalRotation() const { return m_LocalRotation; }

        
        inline Math::Vec3 GetRight() const { return m_Rotation*Math::Vec3(1.0f, 0.0f, 0.0f); }
        inline Math::Vec3 GetUp() const { return m_Rotation*Math::Vec3(0.0f, 1.0f, 0.0f); }
        inline Math::Vec3 GetForward() const { return m_Rotation*Math::Vec3(0.0f, 0.0f, -1.0f); }

        Math::Mat4 GetModelMatrix() const;
        Math::Mat4 GetLocalMatrix() const;
        
        inline Entity& GetEntity() const { return m_Entity; }

        void Rotate(f32 x_angle, f32 y_angle, f32 z_angle);
        void LocalRotate(f32 x_angle, f32 y_angle, f32 z_angle);

        void SetLocalPosition(const Math::Vec3& pos);
        void SetLocalPosition(f32 x, f32 y, f32 z);
        void SetLocalScale(const Math::Vec3& pos);
        void SetLocalScale(f32 x, f32 y, f32 z);
        void SetLocalRotation(const Math::Quat& rotation);

        void SetPosition(const Math::Vec3& pos);
        void SetPosition(f32 x, f32 y, f32 z);
        void SetScale(const Math::Vec3& scale);
        void SetScale(f32 x, f32 y, f32 z);
        void SetRotation(const Math::Quat& rotation);

        void SetParent(Transform* transform);

        void PrintTree(); // temporary for debug
    private:
        void Detach();
        void Attach(Transform* transform);

        void OnGlobalChange();
        void OnLocalChange();
        
        Math::Quat m_Rotation;
        Math::Vec3 m_Position;
        Math::Vec3 m_Scale = Math::Vec3(1.0f);

        Math::Quat m_LocalRotation;
        Math::Vec3 m_LocalPosition;
        Math::Vec3 m_LocalScale = Math::Vec3(1.0f);

        Transform* m_Parent = nullptr;
        Transform* m_NextSibling = nullptr;
        Transform* m_PrevSibling = nullptr;
        Transform* m_FirstChild = nullptr;
        Entity& m_Entity;
    };
}

#endif /* End of FLY_TRANSFORM_H */
