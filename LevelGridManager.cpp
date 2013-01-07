/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "LevelGridManager.h"

LevelGridManager::LevelGridManager(void)
{
    this->m_World = this->m_Engine->GetWorld();

    this->m_CameraBoundaries = new hgeRect();

    // TODO: Find me a place
    LevelGrid::StartX = 28;
    LevelGrid::StartY = 0;

    // Set cell pointers
    this->m_UpperCellPointer = 0;
    this->m_LeftCellPointer  = 0;
    this->m_RightCellPointer = 0;

    // Initialize the grid manager
    this->Initialize();
}

LevelGridManager::~LevelGridManager(void)
{
}

LevelGrid *LevelGridManager::AddGrid(hgeVector offset)
{
    LevelGrid *l;
    hgeVector pos(offset.x + this->m_Engine->GetWidth(), offset.y + this->m_Engine->GetHeight());
    l = new LevelGrid(pos, this->m_CameraBoundaries, 10, 40);
    this->m_World->m_Objects.push_front(l);

    return l;
}

void LevelGridManager::Initialize(void)
{
    this->m_CurrentGrid = this->AddGrid(hgeVector());
    this->m_StartGridOffset = hgeVector();
}

void LevelGridManager::Render(float dt)
{
#ifdef RENDER_BOUNDARIES
    hgeVector a(this->m_CameraBoundaries->x1, this->m_CameraBoundaries->y1);
    hgeVector b(this->m_CameraBoundaries->x2, this->m_CameraBoundaries->y1);
    hgeVector c(this->m_CameraBoundaries->x2, this->m_CameraBoundaries->y2);
    hgeVector d(this->m_CameraBoundaries->x1, this->m_CameraBoundaries->y2);
    this->m_Engine->DrawLine(
        a,
        c,
        0xFFFF0000,
        1.0f
    );
    this->m_Engine->DrawLine(
        d,
        b,
        0xFF00FF00,
        1.0f
    );
#endif
}

void LevelGridManager::Update(float dt)
{
    // Store height and width locally
    unsigned int width  = this->m_Engine->GetWidth();
    unsigned int height = this->m_Engine->GetHeight();

    // Update camera boundaries
    this->m_CameraBoundaries->x1 = -this->m_World->WorldCamera->Position.x;
    this->m_CameraBoundaries->y1 = -this->m_World->WorldCamera->Position.y;
    this->m_CameraBoundaries->x2 = this->m_CameraBoundaries->x1 + width;
    this->m_CameraBoundaries->y2 = this->m_CameraBoundaries->y1 + height;

    // Check if we should add a new cell:
    // [a-b    ] A: Upper left, B: Upper right
    // [|p|    ] C: Lower right, D: Lower left
    // [d-c    ]
    //
    // Determine cell indices of every vertex of the camera space
    // (camera boundaries), in case the specific cell pointer has not yet been
    // incremented to the cell index determined, add a new cell and increase
    // the specific cell pointer

    // Retrieve camera world space vertices
    hgeVector a(this->m_CameraBoundaries->x1, this->m_CameraBoundaries->y1);
    hgeVector b(this->m_CameraBoundaries->x2, this->m_CameraBoundaries->y1);
    hgeVector c(this->m_CameraBoundaries->x2, this->m_CameraBoundaries->y2);
    hgeVector d(this->m_CameraBoundaries->x1, this->m_CameraBoundaries->y2);

    // Determine indices
    int cxl = abs(a.x / width - 1);
    int cxr = (b.x + width * 0.5f) / width - 1;
    int cyu = abs((a.y - height * 0.5f) / height);

    if (cxl == 0 && cxr == 0 && cyu == 0)
    {
        return;
    }

    // Check if we should add a new upper cell
    if (cyu > this->m_UpperCellPointer)
    {
        // Create and add a new cell in the upper region
        hgeVector pos = this->m_StartGridOffset; pos.y -= height * cyu;
        this->AddGrid(pos);

        // Increase the upper cell pointer to indicate that we've just added a cell
        // to the upper region
        ++this->m_UpperCellPointer;

        // In case we move one row up, automagically rebuild the left and right grids
        this->m_LeftCellPointer = 0; this->m_RightCellPointer = 0;
    }
    // Check if we should add a new left cell
    if (cxl > this->m_LeftCellPointer)
    {
        // Create and add a new cell in the left region
        hgeVector pos = this->m_StartGridOffset; pos.x -= width * cxl; pos.y -= height * cyu;
        this->AddGrid(pos);

        // Increase the upper cell pointer to indicate that we've just added a cell
        // to the upper region
        ++this->m_LeftCellPointer;
    }
    // Check if we should add a new right cell
    if (cxr > this->m_RightCellPointer)
    {
        // Create and add a new cell in the right region
        hgeVector pos = this->m_StartGridOffset; pos.x += width * cxr; pos.y -= height * cyu;
        this->AddGrid(pos);

        // Increase the upper cell pointer to indicate that we've just added a cell
        // to the upper region
        ++this->m_RightCellPointer;
    }
}