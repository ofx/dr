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

    this->m_PlayerManager = this->m_Engine->GetWorld()->GetPlayerManager();

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

    // Delete the activators
    for (int i = 0 ; i < this->m_NumActivators ; ++i)
    {
        delete this->m_Activators[i];
    }
    if (this->m_NumActivators > 0)
    {
        delete this->m_Activators;
    }

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

    // Free the left and right index memory
    delete this->m_LeftLevelIndices;
    delete this->m_RightLevelIndices;

    // Free the left and right vertex memory
    delete this->m_LeftLevelVertices;
    delete this->m_RightLevelVertices;
}

void LevelGrid::InitializeLevel(void)
{
    // Give the level vertices some space
    this->m_LeftLevelVertices  = (hgeVector**) malloc(sizeof(hgeVector*) * this->m_NumY);
    this->m_RightLevelVertices = (hgeVector**) malloc(sizeof(hgeVector*) * this->m_NumY);

    // Create some storage for track indices
    this->m_LeftLevelIndices  = (unsigned int*) malloc(sizeof(unsigned int) * this->m_NumY);
    this->m_RightLevelIndices = (unsigned int*) malloc(sizeof(unsigned int) * this->m_NumY);

    // Retrieve the list of players to generate activators
    std::list<Player*> *p = this->m_PlayerManager->GetPlayers();

    // Constructed the grid vertices, now build the level
    for (int i = 0 ; i < this->m_NumY ; ++i)
    {
        // Construct the left and right vertexlist
        this->m_LeftLevelVertices[i]  = this->m_GridVertices[i][StartX];
        this->m_RightLevelVertices[i] = this->m_GridVertices[i][StartX + this->m_TrackWidth];

        // Store indices
        this->m_LeftLevelIndices[i]  = StartX;
        this->m_RightLevelIndices[i] = StartX + this->m_TrackWidth;

        // Only create a "turn" every 2 vertices vertical
        if (i % 2 == 0)
        {
            int lr = rand() % 3 - 1;

            if (StartX + lr < (this->m_NumX - this->m_TrackWidth) && StartX + lr > 0)
            {
                StartX += lr;
            }
        }
    }

    // Create a vector from the list, we need direct lookup
    std::vector<Player*> players(p->begin(), p->end());
    
    // Initialize the number of activators
    this->m_NumActivators = 0;

    // Only add activators whenever there are players
    if (players.size() > 0)
    {
        // Determine the vertical distribution for activators
        int verticalDistribution = this->m_NumY / (players.size() * NUM_ACTIVATORS);

        // Create the activator array
        this->m_Activators = (Activator**) malloc(sizeof(Activator*) * (this->m_NumActivators = NUM_ACTIVATORS * players.size()));

        // This feels a bit hackish...
        std::vector<std::vector<unsigned int>> activatorTypes;
        for (int i = 0 ; i < players.size() ; ++i)
        {
            std::vector<unsigned int> activators;
            for (int j = 0 ; j < NUM_ACTIVATORS ; ++j)
            {
                activators.push_back(j);
            }
            std::random_shuffle(activators.begin(), activators.end());
            activatorTypes.push_back(activators);
        }
        std::random_shuffle(activatorTypes.begin(), activatorTypes.end());

        // Retrieve viewport
        unsigned int width = this->m_Engine->GetWidth();
        unsigned int height = this->m_Engine->GetHeight();

        // Determine deltas
        float dx = width / this->m_NumX;
        float dy = height / this->m_NumY;

        // Define the activator length list
        ACTIVATOR_LENGTH_LIST;

        // Run through the players and then run though activators to make sure that
        // we're never generating two activators for one player in sequence
        int y = 0;
        int n = 0;
        std::vector<std::vector<unsigned int>>::const_iterator it;
        for (int i = 0 ; i < NUM_ACTIVATORS ; ++i)
        {
            int k = 0;
            for (it = activatorTypes.begin() ; it != activatorTypes.end() ; ++it)
            {
                // Fetch a random int between left and right on track y
                int x = (rand() % ((this->m_RightLevelIndices[y] - 1) - (this->m_LeftLevelIndices[y] + 1))) + (this->m_LeftLevelIndices[y] + 1);
        
                // Fetch the activator type pointed to by i
                unsigned int activatorType = (*it).at(i);

                // Offset y in some direction
                srand(x * y);
                int offsetY = rand() % 5;
            
                if (y + offsetY >= this->m_NumY)
                {
                    y -= offsetY;
                }
                else
                {
                    y += offsetY;
                }

                // Get the selected vertex
                hgeVector *vec = this->m_GridVertices[y][x];

                // Create a new activator instance
                Activator *activator = new Activator();

                // Set the type
                activator->ActivatorType = activatorType;

                // Set the owner
                activator->Owner = players.at(k++);

                // In case the activator type is one of type weapon (pickup), add a random weapon to the activator
                if (activatorType == ACTIVATOR_TYPE_WEAPON)
                {
                    // Get a random index
                    int index = rand() % NUM_WEAPONS;

                    switch (index)
                    {
                        case WEAPON_INDEX_NEOSHOOTER:
                            activator->Data = new Neoshooter(activator->Owner, this->m_Engine->GetWorld());
                            break;
                        case WEAPON_INDEX_TRISCEPTRE:
                            activator->Data = new Trisceptre(activator->Owner, this->m_Engine->GetWorld());
                            break;
                    }
                }
            
                // Define the vertices
                activator->Vertices[0] = *vec; activator->Vertices[1] = *vec;
                activator->Vertices[2] = *vec; activator->Vertices[3] = *vec;

                // Offset the vertices conform winding
                activator->Vertices[1].x -= dx;
                activator->Vertices[2].x -= dx; activator->Vertices[2].y -= dy * ACTIVATOR_LENGTH_LIST_NAME[activatorType];
                activator->Vertices[3].y -= dy * ACTIVATOR_LENGTH_LIST_NAME[activatorType];

                // Add the activator
                this->m_Activators[n++] = activator;
            }

            // Increase the vertical pointer
            y += verticalDistribution;
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
        shape->collision_type = COLLISION_TYPE_LEVEL;
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
        shape->collision_type = COLLISION_TYPE_LEVEL;
        cpSpaceAddStaticShape(space, shape);
        cpBodyActivateStatic(this->m_Body, shape);
        this->m_Shapes.push_back(shape);
    }

    // Create the activator physics shapes
    for (int i = 0 ; i < this->m_NumActivators ; ++i)
    {
        Activator *activator = this->m_Activators[i];

        // Create the cpVects
        cpVect *verts = (cpVect*) malloc(sizeof(cpVect) * 4);
        verts[3].x = activator->Vertices[0].x; verts[3].y = activator->Vertices[0].y;
        verts[2].x = activator->Vertices[1].x; verts[2].y = activator->Vertices[1].y;
        verts[1].x = activator->Vertices[2].x; verts[1].y = activator->Vertices[2].y;
        verts[0].x = activator->Vertices[3].x; verts[0].y = activator->Vertices[3].y;

        // Create and add the poly shape
        cpShape *shape = cpPolyShapeNew(this->m_Body, 4, verts, cpvzero);
        shape->e = 1.0; shape->u = 1.0;
        shape->sensor         = true;
        shape->collision_type = COLLISION_TYPE_ACTIVATOR;
        shape->data           = activator;
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

    // Render the activators
    for (int i = 0 ; i < this->m_NumActivators ; ++i)
    {
        Activator *activator = this->m_Activators[i];

        // Defint the inner color
        DWORD color = activator->Owner->GetColor();
        color &= 0xB4FFFFFF;

        // Define the outline color
        DWORD colorLine = activator->Owner->GetColor();
        color &= 0x64FFFFFF;
    
        // Render the quad
        hgeQuad *activatorQuad = new hgeQuad();
        activatorQuad->tex    = 0;
        activatorQuad->blend  = BLEND_DEFAULT_Z;
        activatorQuad->v[0].x = activator->Vertices[0].x; activatorQuad->v[0].y = activator->Vertices[0].y; activatorQuad->v[0].col = color;
        activatorQuad->v[1].x = activator->Vertices[1].x; activatorQuad->v[1].y = activator->Vertices[1].y; activatorQuad->v[1].col = color;
        activatorQuad->v[2].x = activator->Vertices[2].x; activatorQuad->v[2].y = activator->Vertices[2].y; activatorQuad->v[2].col = color;
        activatorQuad->v[3].x = activator->Vertices[3].x; activatorQuad->v[3].y = activator->Vertices[3].y; activatorQuad->v[3].col = color;

        this->m_Engine->GetHge()->Gfx_RenderQuad(activatorQuad);
        delete activatorQuad;

        // Render the outline
        this->m_Engine->DrawLine(
            activator->Vertices[0],
            activator->Vertices[1],
            colorLine,
            this->m_LineWidth
        );
        this->m_Engine->DrawLine(
            activator->Vertices[1],
            activator->Vertices[2],
            colorLine,
            this->m_LineWidth
        );
        this->m_Engine->DrawLine(
            activator->Vertices[2],
            activator->Vertices[3],
            colorLine,
            this->m_LineWidth
        );
        this->m_Engine->DrawLine(
            activator->Vertices[3],
            activator->Vertices[0],
            colorLine,
            this->m_LineWidth
        );

        // Render individual markers
        switch (activator->ActivatorType)
        {
            case ACTIVATOR_TYPE_BOOST:
                this->RenderBoostActivatorMarker(activator);
                break;
            case ACTIVATOR_TYPE_FIRE:
                this->RenderFireActivatorMarker(activator);
                break;
            case ACTIVATOR_TYPE_WEAPON:
                this->RenderWeaponActivatorMarker(activator);
                break;
        }
    }
}

void LevelGrid::RenderWeaponActivatorMarker(Activator *activator)
{
    if (activator->Data != 0)
    {
        this->m_Engine->GetDefaultFont()->printf(
            (activator->Vertices[0].x + activator->Vertices[1].x) / 2,
            activator->Vertices[0].y,
            HGETEXT_CENTER,
            "%s",
            ((Weapon*)activator->Data)->WeaponShortName.c_str()
        );
    }
}

void LevelGrid::RenderFireActivatorMarker(Activator *activator)
{
    // Define the color
    DWORD color = activator->Owner->GetColor();
    color &= 0x64FFFFFF;

    // Render a cross in the middle of the activator
    this->m_Engine->DrawLine(
        activator->Vertices[0],
        activator->Vertices[2],
        color,
        3.0f
    );
    this->m_Engine->DrawLine(
        activator->Vertices[1],
        activator->Vertices[3],
        color,
        3.0f
    );
}

void LevelGrid::RenderBoostActivatorMarker(Activator *activator)
{
    // Define the color
    DWORD color = activator->Owner->GetColor();
    color &= 0x64FFFFFF;

    // Define midpoint
    hgeVector midpoint;
    midpoint.y = activator->Vertices[0].y;
    midpoint.x = (activator->Vertices[0].x + activator->Vertices[1].x) / 2;

    // Render a cross in the middle of the activator
    this->m_Engine->DrawLine(
        activator->Vertices[3],
        midpoint,
        color,
        3.0f
    );
    this->m_Engine->DrawLine(
        activator->Vertices[2],
        midpoint,
        color,
        3.0f
    );
}

void LevelGrid::Update(float dt)
{

}