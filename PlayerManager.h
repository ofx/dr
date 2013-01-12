/**
 * DroidRacers
 * by Marlon Etheredge
 */

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

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);

    void RenderText(hgeFont *font, float dt);
};