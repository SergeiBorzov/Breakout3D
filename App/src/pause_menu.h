#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "fly.h"

class Game;
class PauseMenu: public Fly::NativeScript {
public:
    PauseMenu(Fly::Entity& entity, Game* game);

    void Show();
    void Hide();

    virtual ~PauseMenu() {}
private:
    Game* m_Game;

    Fly::Entity* m_ResumeEntity;
    Fly::Entity* m_MainMenuEntity;
    Fly::Entity* m_ExitEntity;
};

#endif /* End of PAUSE_MENU_H */