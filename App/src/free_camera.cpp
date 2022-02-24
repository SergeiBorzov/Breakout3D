#include "free_camera.h"

using namespace Fly;
void FreeCamera::OnStart() {
    i32 width = g_Window->GetWidth();
    i32 height = g_Window->GetHeight();
    CameraComponent* cam = entity.AddComponent<CameraComponent>();
    cam->SetProjection(Math::Perspective(60.0f, (f32)(width)/height, 0.1f, 100.0f));
    CameraComponent::s_MainCamera = cam;

    transform.SetPosition({0.0f, 30.0f, 0.0f});
    transform.SetRotation(Math::Quat::Euler(-89.0f, 0.0f, 0.0f));
}

void FreeCamera::OnUpdate(f32 deltaTime) {
    UpdatePosition(deltaTime);
    UpdateRotation();
}

void FreeCamera::UpdateRotation() {
    Input::GetMousePosition(&m_MousePosition.x, &m_MousePosition.y);
    if (m_FirstMouse) {
        m_PrevMousePosition = m_MousePosition;
        m_FirstMouse = false;
    }

    Math::Vec2 deltaPos = m_PrevMousePosition - m_MousePosition;
    m_PrevMousePosition = m_MousePosition;

    Math::Vec3 angles;
    Math::Quat::ToEuler(transform.GetRotation(), angles);

    angles.x += deltaPos.y*m_Sensitivity;
    angles.y += deltaPos.x*m_Sensitivity;

    angles.x = Math::Clamp(angles.x, -89.0f, 89.0f);
    transform.SetRotation(Math::Quat::Euler(angles));
}

void FreeCamera::UpdatePosition(f32 deltaTime) {
    Math::Vec3 pos = transform.GetPosition();

    if (Input::GetKey(Key::W)) {
        pos += transform.GetForward()*m_Speed*deltaTime;
    } else if (Input::GetKey(Key::S)) {
        pos -= transform.GetForward()*m_Speed*deltaTime;
    }

    if (Input::GetKey(Key::A)) {
        pos -= transform.GetRight()*m_Speed*deltaTime;
    }   else if (Input::GetKey(Key::D)) {
        pos += transform.GetRight()*m_Speed*deltaTime;
    }

    if (Input::GetKey(Key::Space)) {
        Math::Vec3 up = transform.GetUp();
        pos += transform.GetUp()*m_Speed*deltaTime;
    } else if (Input::GetKey(Key::LeftShift)) {
        pos -= transform.GetUp()*m_Speed*deltaTime;
    }

    transform.SetPosition(pos);
}