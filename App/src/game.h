#include "fly.h"

class PauseMenu;
class Game: public Fly::App {
public:
    enum State {
        SplashScreenState,
        MainMenuState,
        Pause,
        Active,
        GameOver,
        Exit
    };

    virtual void Start() override;
    virtual void Update() override;
    virtual ~Game() {}

    void LoadMainMenu();
    void NewGame();
    void ResumeGame();
    void CloseGame();
    void ResetMainMenu();
    void GameOverState();
private:
    void LoadSplashScreen();
    void LoadMainResources();

    void PauseState();
    void ActiveState();

    Fly::Scene m_Scene;
    PauseMenu* m_PauseMenu = nullptr;

    State m_CurrentState = State::SplashScreenState;
    bool m_Pause = false;
    bool m_PauseNotReleased = false;
};