/**
 * DroidRacers
 * by Marlon Etheredge
 */

#define PLAYER_COLOR_SET_NAME  colorList
#define PLAYER_COLOR_SET       DWORD colorList[7] = {0xFFFFA0FF, 0xFF6CCCBE, 0xFF7F2A7F, 0xFF6CCCBD, 0xFFFFF0B9, 0xFF70B24C, 0xFF992323};
#define DEFAULT_PLAYER_SPAWN_X 400.0f
#define DEFAULT_PLAYER_SPAWN_Y 600.0f
#define PLAYER_SPAWN_OFFSET_Y  10.0f

#define PLAYER_WARNING_DIST    250.0f
#define PLAYER_REPOSITION_DIST 300.0f

#define PLAYER_REPOSITION_HEALTH_DECREASE 0.15f

#pragma once

#include <algorithm>
#include <functional>
#include <list>

struct PlayerSort
{
    bool operator()(Player *p1, Player *p2) const
    {
        return p1->m_Position.y < p2->m_Position.y;
    }
};

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
    void RemovePlayer(Player *player);

    float *FindPlayerPositionCentroid(void);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);

    void RenderText(hgeFont *font, float dt);
};