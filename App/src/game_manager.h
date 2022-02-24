#ifndef FLY_GAME_MANAGER_H
#define FLY_GAME_MANAGER_H

#include "fly.h"

/* Counts lives, handles power ups */

class Game;
class FreeCamera;
class Paddles;
class Ball;
class CornerLight;
class MainLight;
class Tower;

class GameManager: public Fly::NativeScript {
public:
    static GameManager* GetInstance() {
        return instance;
    }

    GameManager(Fly::Entity& entity, Game* game): Fly::NativeScript(entity), m_Game(game) {}
    virtual ~GameManager() {}

    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;
    void CreateScene();
    void ResetBall();

    // Bonuses
    void AddPoints(u32 points);
    inline void AddLife(i32 count) { m_LivesLeft += count; }
    void IncreasePaddles();
    void DecreasePaddles();
    void IncreaseBallSpeed();
    void DecreaseBallSpeed();
    void TemporaryPierce();
private:
    CornerLight* m_CornerLights[4] = {nullptr}; // ptrs to scene lights spot and 4 point lights
    Fly::CameraComponent* m_Cameras[2] = {nullptr};
    Game* m_Game;
    MainLight* m_SpotLight = nullptr;
    Paddles* m_Paddles = nullptr; // ptr to paddles to change size and speed
    Tower* m_Tower = nullptr;
    Fly::Entity* m_MainPaddle = nullptr;
    Ball* m_Ball = nullptr; // ptr to handle the ball
    Fly::UI::Label* m_LifeLabel = nullptr;
    Fly::UI::Label* m_GameOverLabel = nullptr;

    i32 m_LivesLeft = 3;
    i32 m_CurrentCameraIndex = 0;
    bool m_BallReleased = false;

    static GameManager* instance;
};

#endif /* End of FLY_GAME_MANAGER_H */