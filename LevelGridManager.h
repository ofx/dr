/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include "gameobject.h"

class LevelGrid;
class World;

class LevelGridManager : public GameObject
{
private:
    World *m_World;

    int m_UpperCellPointer;
    int m_LeftCellPointer;
    int m_RightCellPointer;

    LevelGrid *m_CurrentGrid;
    hgeVector m_StartGridOffset;

    hgeRect *m_CameraBoundaries;
public:
    LevelGridManager(void);
    ~LevelGridManager(void);

    LevelGrid *AddGrid(hgeVector offset, bool addLevel);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);
};

