/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <map>

class LevelGrid : public GameObject
{
private:
    HTEXTURE m_Texture;
    HTEXTURE m_StarfieldTexture;

    hgeQuad *m_BackgroundQuad;

    cpBody             *m_Body;
    std::list<cpShape*> m_Shapes;

    int m_ReleaseTimer;

    unsigned int m_NumX;
    unsigned int m_NumY;

    unsigned int m_TrackWidth;
    
    float m_LineWidth;

    hgeVector m_Offset;
    hgeRect   m_Boundaries;
    hgeRect  *m_CameraBoundaries;

    hgeVector **m_LeftLevelVertices;
    hgeVector **m_RightLevelVertices;
    hgeVector ***m_GridVertices;
public:
    static unsigned int StartX;
    static unsigned int StartY;

    LevelGrid(hgeVector offset, hgeRect *cameraBoundaries, int trackWidth, unsigned int splitfactor);
    ~LevelGrid(void);

    void Initialize(void);
    void InitializeLevel(void);
    void InitializeLevelPhysics(void);
    void Render(float dt);
    void Update(float dt);

    friend class LevelGridManager;
};

