/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

unsigned int LevelGrid::StartX = 0;
unsigned int LevelGrid::StartY = 0;

LevelGrid::LevelGrid(hgeVector offset, hgeRect *cameraBoundaries, int trackWidth, unsigned int splitfactor, bool hasLevel)
{
    this->m_NumX         = splitfactor;
    this->m_NumY         = splitfactor;
    this->m_LineWidth    = 3.0f;

    this->m_HasLevel     = hasLevel;

    this->m_ReleaseTimer = 0;

    this->m_TrackWidth = trackWidth;

    this->m_CameraBoundaries = cameraBoundaries;

    // Set offset
    this->m_Offset = offset;

    // Initialize the grid
    this->Initialize();
}

LevelGrid::~LevelGrid(void)
{
    delete this->m_BackgroundQuad;
    
    // Remove grid vertices
    for (int y = 0 ; y <= this->m_NumY ; ++y)
    {
        for (int x = 0 ; x <= this->m_NumX ; ++x)
        {
            delete this->m_GridVertices[y][x];
        }
        delete this->m_GridVertices[y];
    }
    delete this->m_GridVertices;

    // Free the physics shapes and body
    std::list<cpShape*>::const_iterator it;
    cpSpace *space = this->m_Engine->GetWorld()->GetSpace();
    for (it = this->m_Shapes.begin() ; it != this->m_Shapes.end() ; ++it)
    {
        cpSpaceRemoveShape(space, *it);
        cpShapeFree(*it);
    }
    this->m_Shapes.clear();
    delete this->m_Body;

    // Free the left and right vertex memory
    delete this->m_LeftLevelVertices;
    delete this->m_RightLevelVertices;
}

void LevelGrid::InitializeLevel(void)
{
    // Give the level vertices some space
    this->m_LeftLevelVertices  = (hgeVector**) malloc(sizeof(hgeVector*) * this->m_NumY);
    this->m_RightLevelVertices = (hgeVector**) malloc(sizeof(hgeVector*) * this->m_NumY);

    // Constructed the grid vertices, now build the level
    for (int i = 0 ; i < this->m_NumY ; ++i)
    {
        this->m_LeftLevelVertices[i]  = this->m_GridVertices[i][StartX];
        this->m_RightLevelVertices[i] = this->m_GridVertices[i][StartX + this->m_TrackWidth];

        if (i % 2 == 0)
        {
            int lr = rand() % 3 - 1;

            if (StartX + lr < (this->m_NumX - this->m_TrackWidth) && StartX + lr > 0)
            {
                StartX += lr;
            }
        }
    }
}

void LevelGrid::InitializeLevelPhysics(void)
{
    // Fetch the physics space
    cpSpace *space = this->m_Engine->GetWorld()->GetSpace();

    // Add the physics body
    this->m_Body = cpBodyNewStatic();
        
    // Add the physics shapes
    for (int i = 0 ; i < this->m_NumY - 1 ; ++i)
    {
        hgeVector *vec  = this->m_LeftLevelVertices[i];
        hgeVector *nvec = this->m_LeftLevelVertices[i + 1];

        cpShape *shape = cpSegmentShapeNew(this->m_Body, cpv(vec->x, vec->y), cpv(nvec->x, nvec->y), 0.0f);
        shape->e = 1.0; shape->u = 1.0;
        cpSpaceAddStaticShape(space, shape);
        cpBodyActivateStatic(this->m_Body, shape);
        this->m_Shapes.push_back(shape);
    }
    for (int i = 0 ; i < this->m_NumY - 1 ; ++i)
    {
        hgeVector *vec  = this->m_RightLevelVertices[i];
        hgeVector *nvec = this->m_RightLevelVertices[i + 1];
        
        cpShape *shape = cpSegmentShapeNew(this->m_Body, cpv(vec->x, vec->y), cpv(nvec->x, nvec->y), 0.0f);
        shape->e = 1.0; shape->u = 1.0;
        cpSpaceAddStaticShape(space, shape);
        cpBodyActivateStatic(this->m_Body, shape);
        this->m_Shapes.push_back(shape);
    }
}

void LevelGrid::Initialize(void)
{
    hgeQuad *mQuad;

    // Load the texture
    if (!(this->m_StarfieldTexture = this->m_Hge->Texture_Load("data/Starfield.png")))
	{
		MessageBox(NULL, "Can't load Starfield.png texture.", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		this->m_Engine->Shutdown();
		
        return;
	}

    // Retrieve viewport
    unsigned int width = this->m_Engine->GetWidth();
    unsigned int height = this->m_Engine->GetHeight();

    hgeVector backOffset;
    backOffset.x = this->m_Offset.x - width;
    backOffset.y = this->m_Offset.y - height;

    // First add the background (starfield)
    {
        mQuad = new hgeQuad();
        mQuad->tex   = this->m_StarfieldTexture;
        mQuad->blend = BLEND_DEFAULT_Z;
        mQuad->v[0].col = mQuad->v[1].col = mQuad->v[2].col = mQuad->v[3].col = ARGB(255, 255, 255, 255);
 
        mQuad->v[0].x = backOffset.x;
        mQuad->v[0].y = backOffset.y;
        mQuad->v[0].tx = 0;
        mQuad->v[0].ty = 0;
        this->m_Boundaries.x1 = mQuad->v[0].x;
        this->m_Boundaries.y1 = mQuad->v[0].y;
 
        mQuad->v[1].x = backOffset.x + width;
        mQuad->v[1].y = backOffset.y;
        mQuad->v[1].tx = 1;
        mQuad->v[1].ty = 0;
 
        mQuad->v[2].x = backOffset.x + width;
        mQuad->v[2].y = backOffset.y + height;
        mQuad->v[2].tx = 1;
        mQuad->v[2].ty = 1;
        this->m_Boundaries.x2 = mQuad->v[2].x;
        this->m_Boundaries.y2 = mQuad->v[2].y;
 
        mQuad->v[3].x = backOffset.x;
        mQuad->v[3].y = backOffset.y + height;
        mQuad->v[3].tx = 0;
        mQuad->v[3].ty = 1;

        this->m_BackgroundQuad = mQuad;
    }

#ifdef false
    char b[100];
    sprintf(
        b, 
        "%4.2f,%4.2f\n%4.2f,%4.2f\n%4.2f,%4.2f\n%4.2f,%4.2f\n", 
        this->m_Boundaries.x1,
        this->m_Boundaries.y1,
        this->m_Boundaries.x2,
        this->m_Boundaries.y1,
        this->m_Boundaries.x2,
        this->m_Boundaries.y2,
        this->m_Boundaries.x1,
        this->m_Boundaries.y2
    );
    OutputDebugStringA(b);
#endif

    // Allocate
    // Achtung: Adding one extra to fill the complete screen
    this->m_GridVertices = (hgeVector***) malloc((this->m_NumY + 1) * sizeof(hgeVector*));
    for (int y = 0 ; y <= this->m_NumY ; ++y)
    {
        this->m_GridVertices[y] = (hgeVector**) malloc((this->m_NumX + 1) * sizeof(hgeVector));
    }

    // Determine deltas
    float dx = width / this->m_NumX;
    float dy = height / this->m_NumY;

    // Define the grid vertices
    for (int y = 0 ; y <= this->m_NumY ; ++y)
    {
        for (int x = 0 ; x <= this->m_NumX ; ++x)
        {
            this->m_GridVertices[y][x]    = new hgeVector();
            this->m_GridVertices[y][x]->x = this->m_Offset.x - (x * dx);
            this->m_GridVertices[y][x]->y = this->m_Offset.y - (y * dy);
        }
    }

    // Initialize level
    if (this->m_HasLevel)
    {
        this->InitializeLevel();
        this->InitializeLevelPhysics();
    }
}

void LevelGrid::Render(float dt)
{
    // Only render if the grid intersects the camera space
    if (!this->m_Boundaries.Intersect(this->m_CameraBoundaries))
    {
        ++this->m_ReleaseTimer;
        if (this->m_ReleaseTimer > 1000)
        {
            this->m_Engine->GetWorld()->RemoveGameObject(this);
        }
        return;
    }

    // Render the background quad
    this->m_Hge->Gfx_RenderQuad(this->m_BackgroundQuad);

    // Temp vector storage
    hgeVector *a = 0;

    // Render vertical
    for (int y = 0 ; y <= this->m_NumY ; ++y)
    {
        a = 0;

        for (int x = 0 ; x <= this->m_NumX ; ++x)
        {
            hgeVector *b = this->m_GridVertices[y][x];
            if (a != 0)
            {
                this->m_Engine->DrawLine(
                    *a,
                    *b,
                    0x776CCCBD,
                    this->m_LineWidth
                );
            }

            a = b;    
        }
    }

    // Render horizontal
    for (int x = 0 ; x <= this->m_NumX ; ++x)
    {
        a = 0;

        for (int y = 0 ; y <= this->m_NumY ; ++y)
        {
            hgeVector *b = this->m_GridVertices[y][x];
            if (a != 0)
            {
                this->m_Engine->DrawLine(
                    *a,
                    *b,
                    0x776CCCBD,
                    this->m_LineWidth
                );
            }

            a = b;    
        }
    }

    // Render the level
    if (this->m_HasLevel)
    {
        for (int i = 0 ; i < this->m_NumY - 1 ; ++i)
        {
            hgeVector *vec  = this->m_LeftLevelVertices[i];
            hgeVector *nvec = this->m_LeftLevelVertices[i + 1];

            this->m_Engine->DrawLine(
                *vec,
                *nvec,
                0xFF70B24C,
                this->m_LineWidth
            );
        }
        for (int i = 0 ; i < this->m_NumY - 1 ; ++i)
        {
            hgeVector *vec  = this->m_RightLevelVertices[i];
            hgeVector *nvec = this->m_RightLevelVertices[i + 1];

            this->m_Engine->DrawLine(
                *vec,
                *nvec,
                0xFF70B24C,
                this->m_LineWidth
            );
        }
    }
}

void LevelGrid::Update(float dt)
{

}