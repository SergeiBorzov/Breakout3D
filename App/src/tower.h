#ifndef TOWER_H
#define TOWER_H

#include <array>

#include "fly.h"

#include "brick.h"

class Tower: public Fly::NativeScript {
public:
    Tower(Fly::Entity& entity): Fly::NativeScript(entity) {}

    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;
    void ReleaseBrick(i32 index);
    void Replace(i32 index);
    void SetBrickTransparency(bool value);
    void CreateNew();

    virtual ~Tower() {}
private:
    std::array<Brick*, 3*24> m_Bricks;
    i32 m_BricksAlive = 3*24;
    f32 m_Scales[3] = {1.0f, 0.83333f, 0.6666f};
    f32 m_DelayCount = 0.0f;
};

#endif /* End of TOWER_H */