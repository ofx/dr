/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <list>

// Deprecated
#pragma deprecated(Level)
class Level : public GameObject
{
private:
    std::list<cpVect*> m_LeftVertexList;
    std::list<cpVect*> m_RightVertexList;
public:
    Level(void);
    ~Level(void);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);
};

