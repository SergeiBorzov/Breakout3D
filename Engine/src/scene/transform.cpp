#include "../core/log.h"
#include "entity.h"

namespace Fly {

    Math::Mat4 Transform::GetModelMatrix() const {
        Math::Mat3 rot = Math::Quat::RotationMatrix(m_Rotation);
        
        return Math::Mat4({m_Scale.x*rot[0][0], m_Scale.x*rot[1][0], m_Scale.x*rot[2][0], 0.0f},
                          {m_Scale.y*rot[0][1], m_Scale.y*rot[1][1], m_Scale.y*rot[2][1], 0.0f},
                          {m_Scale.z*rot[0][2], m_Scale.z*rot[1][2], m_Scale.z*rot[2][2], 0.0f},
                          {m_Position.x, m_Position.y, m_Position.z, 1.0f}); 
    }

    Math::Mat4 Transform::GetLocalMatrix() const {
        return Math::Scale(m_LocalScale)*Math::Rotation(m_LocalRotation)*Math::Translate(m_LocalPosition);
    }

    void Transform::OnGlobalChange() {
        if (!m_Parent) {
            m_LocalPosition = m_Position;
            m_LocalScale = m_Scale;
            m_LocalRotation = m_Rotation;
	    }
        else {
            Math::Quat parentInverse = Math::Inverse(m_Parent->m_Rotation);
            m_LocalPosition = parentInverse*(m_Position - m_Parent->m_Position);
            m_LocalScale = m_Scale/m_Parent->m_Scale;
            m_LocalRotation = Math::Normalize(m_Rotation*parentInverse);
        }

        if (m_FirstChild) {
            Transform* p = m_FirstChild;
            while(p) {
                p->OnLocalChange();
                p = p->m_NextSibling;
            }
        }
    }

    void Transform::OnLocalChange() {
        if (m_Parent) {
            m_Rotation = Math::Normalize(m_Parent->m_Rotation*m_LocalRotation);
            m_Scale = m_Parent->m_Scale*m_LocalScale;
            m_Position = m_Parent->m_Rotation*(m_Parent->m_Scale*m_LocalPosition) + m_Parent->m_Position; //m_Parent->m_Rotation*(m_Parent->m_Scale*m_LocalPosition);
        }

        if (m_FirstChild) {
            Transform* p = m_FirstChild;
            while(p) {
                p->OnLocalChange();
                p = p->m_NextSibling;
            }
        }

    }

    void Transform::Detach() {
        if (!m_Parent) {
            return;
        }

        if (m_Parent->m_FirstChild == this) {
            if (m_NextSibling) {
                m_Parent->m_FirstChild = m_NextSibling;
                m_NextSibling->m_PrevSibling = nullptr;
            }
            else {
                m_Parent->m_FirstChild = nullptr;
            }
            m_Parent = nullptr;
            return;
        }
        else {
            Transform* p = m_Parent->m_FirstChild;

            while (p) {
                if (p == this)
                    break;
                p = p->m_NextSibling;
            }

            // At this point p shouldn't be nullptr
            p->m_PrevSibling->m_NextSibling = p->m_NextSibling;

            if (p->m_NextSibling) {
                p->m_NextSibling->m_PrevSibling = p->m_PrevSibling;
            }
        }

        m_Parent = nullptr;
    }

    void Transform::Attach(Transform* transform) {
        m_Parent = transform;

        if (!m_Parent->m_FirstChild) {
            m_Parent->m_FirstChild = this;
            return;
        }

        Transform* firstChild = m_Parent->m_FirstChild;
        m_Parent->m_FirstChild = this;
        m_NextSibling = firstChild;
        firstChild->m_PrevSibling = this;
    }
    
    void Transform::SetParent(Transform* transform) {
        if (transform == m_Parent) {
            return;
        }
        Detach();
        if (transform) {
            Attach(transform);
        }
        OnGlobalChange();
    }

    void Transform::PrintTree() {
        FLY_LOG("%s", m_Entity.name.c_str());
        Transform* p = m_FirstChild;
        while (p) {
            p->PrintTree();
            p = p->m_NextSibling;
        }
    }

    
    void Transform::SetPosition(const Math::Vec3& pos) {
        m_Position = pos;
        OnGlobalChange();
    }

    void Transform::SetPosition(f32 x, f32 y, f32 z) {
        m_Position = {x, y, z};
        OnGlobalChange();
    }

    void Transform::SetLocalPosition(const Math::Vec3& pos) {
        m_LocalPosition = pos;
        OnLocalChange();
    }

    void Transform::SetLocalPosition(f32 x, f32 y, f32 z) {
        m_LocalPosition = {x, y, z};
        OnLocalChange();
    }

    void Transform::SetScale(const Math::Vec3& scale) {
        m_Scale = scale;
        OnGlobalChange();
    }

    void Transform::SetScale(f32 x, f32 y, f32 z) {
        m_Scale = {x, y, z};
        OnGlobalChange();
    }

    void Transform::SetLocalScale(const Math::Vec3& scale) {
        m_LocalScale = scale;
        OnLocalChange();
    }

    void Transform::SetLocalScale(f32 x, f32 y, f32 z) {
        m_LocalScale = {x, y, z};
        OnLocalChange();
    }

    void Transform::SetRotation(const Math::Quat& rotation) {
        m_Rotation = rotation;
        OnGlobalChange();
    }

    void Transform::SetLocalRotation(const Math::Quat& rotation) {
        m_LocalRotation = rotation;
        OnLocalChange();
    }

    void Transform::Rotate(f32 x_angle, f32 y_angle, f32 z_angle) {
        Math::Quat rot = Math::Quat::Euler(x_angle, y_angle, z_angle);
        SetRotation(rot*m_Rotation);
    }
}

