/**
 * DroidRacers
 * by Marlon Etheredge
 */

#define PLAYER_COLOR_SET_NAME  colorList
#define PLAYER_COLOR_SET       DWORD colorList[7] = {0xFFFFA0FF, 0xFF6CCCBE, 0xFF7F2A7F, 0xFF6CCCBD, 0xFFFFF0B9, 0xFF70B24C, 0xFF992323};
#define DEFAULT_PLAYER_SPAWN_X 400.0f
#define DEFAULT_PLAYER_SPAWN_Y 600.0f
#define PLAYER_SPAWN_OFFSET_Y  10.0f

#pragma once

#include <algorithm>
#include <functional>
#include <list>

class PlayerManager : public GameObject
{
private:
    World *m_World;

    std::list<Player*> *m_Players;
public:
    PlayerManager(void);
    ~PlayerManager(void);

    std::list<Player*> *GetPlayers(void)
    {
        return this->m_Players;
    }

    Player *NewPlayer(void);
    Player *RemovePlayer(void);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);

    void RenderText(hgeFont *font, float dt);
};