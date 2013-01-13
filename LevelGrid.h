/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <map>

#define NUM_ACTIVATORS       2
#define ACTIVATOR_TYPE_FIRE  0
#define ACTIVATOR_TYPE_BOOST 1

#define ACTIVATOR_LENGTH_LIST_NAME activatorLengthList
#define ACTIVATOR_LENGTH_LIST      unsigned int activatorLengthList[NUM_ACTIVATORS] = {1, 3}

typedef struct Activator
{
    unsigned int  ActivatorType;
    hgeVector     Vertices[4];
    Player       *Owner;
} Activator;

class LevelGrid : public GameObject
{
private:
    HTEXTURE m_Texture;
    HTEXTURE m_StarfieldTexture;

    hgeQuad *m_BackgroundQuad;

    PlayerManager *m_PlayerManager;

    cpBody             *m_Body;
    std::list<cpShape*> m_Shapes;

    int m_ReleaseTimer;

    unsigned int m_NumX;
    unsigned int m_NumY;

    unsigned int m_TrackWidth;

    bool m_HasLevel;
    
    float m_LineWidth;

    unsigned int m_NumActivators;
    Activator **m_Activators;

    hgeVector m_Offset;
    hgeRect   m_Boundaries;
    hgeRect  *m_CameraBoundaries;

    unsigned int *m_LeftLevelIndices;
    unsigned int *m_RightLevelIndices;

    hgeVector **m_LeftLevelVertices;
    hgeVector **m_RightLevelVertices;
    hgeVector ***m_GridVertices;
public:
    static unsigned int StartX;
    static unsigned int StartY;

    LevelGrid(hgeVector offset, hgeRect *cameraBoundaries, int trackWidth, unsigned int splitfactor, bool hasLevel);
    ~LevelGrid(void);

    void RenderFireActivatorMarker(Activator *activator);
    void RenderBoostActivatorMarker(Activator *activator);

    void Initialize(void);
    void InitializeLevel(void);
    void InitializeLevelPhysics(void);
    void Render(float dt);
    void Update(float dt);

    friend class LevelGridManager;
};

