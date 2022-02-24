#include "tower.h"

using namespace Fly;

void Tower::ReleaseBrick(i32 index) {
    m_Bricks[index] = nullptr;
    m_BricksAlive--;
}

void Tower::OnStart() {
    CreateNew();
}

void Tower::SetBrickTransparency(bool value) {
    for (int i = 0; i < 3*24; i++) {
        if (m_Bricks[i]){
            m_Bricks[i]->SetTransparency(value);
        }
    }
}

void Tower::CreateNew() {
    f32 radius = 4.5f;
    m_BricksAlive = 3*24;
    for (i32 i = 0; i < 3; i++) {
        f32 deltaAngle = 2.0f*Math::PI/24.0f;
        for (i32 j = 0; j < 24; j++) {
            Entity* brick = scene.CreateEntity("Brick");

            i32 randomValue = Math::RandomInt(0, 99);

            Brick* brickComponent = nullptr;
            if (randomValue < 20) {
                brickComponent = brick->AddComponent<BonusBrick>(this, i*24 + j);
                m_Bricks[i*24 + j] = brickComponent;
            }
            else if (randomValue >= 20 && randomValue < 50) {
                brickComponent = brick->AddComponent<ToughBrick>(this, i*24 + j);
                m_Bricks[i*24 + j] = brickComponent;
            }
            else {
               brickComponent = brick->AddComponent<Brick>(this, i*24 + j);
                m_Bricks[i*24 + j] = brickComponent;
            }
            
            brick->transform.SetScale(m_Scales[i], 1.0f, m_Scales[i]);
            brick->transform.SetPosition((radius - (f32)i*0.75f)*Math::Sin(j*deltaAngle), (f32)i, (radius - (f32)i*0.75f)*Math::Cos(j*deltaAngle));
            brick->transform.Rotate(0.0f, Math::Degrees(j*deltaAngle), 0.0f);
            brick->transform.SetParent(&transform);
            brickComponent->StartAppearing();
        }
    }
}

void Tower::Replace(i32 index) {
    m_Bricks[index] = m_Bricks[index + 24];
    m_Bricks[index + 24] = nullptr;
}


void Tower::OnUpdate(f32 deltaTime) {
    for (i32 i = 0; i < 3; i++) {
        for (i32 j = 0; j < 24; j++) {
            Brick* brick = m_Bricks[i*24 + j];
            if (brick && i > 0 ) {
                brick->GetCollider()->enabled = false;
            }
            else if (brick && i == 0 && !brick->IsDissolving() && !brick->IsAppearing()) {
                brick->GetCollider()->enabled = true;
            }
            if (!brick && i < 2) {
                if (m_Bricks[(i+1)*24 + j] && !m_Bricks[(i+1)*24 + j]->IsReplacing())
                    m_Bricks[(i+1)*24 + j]->StartReplacement(i*24 + j, m_Scales[i]);
            }
        }
    }

    if (m_BricksAlive == 0) {
        m_DelayCount += deltaTime;
        if (m_DelayCount > 0.25f) {
            CreateNew();
        }
    }
}