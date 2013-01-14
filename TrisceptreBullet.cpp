#include "StdAfx.h"

#include "TrisceptreBullet.h"

TrisceptreBullet::TrisceptreBullet(Player *owner) : Bullet(owner)
{
    this->m_Speed = 2.0f;

    // Set the direction vector to the current direction of the player
    this->m_DirectionVector = owner->GetDirectionVector();

    // Initialize the bullet
    this->Initialize();
}

TrisceptreBullet::~TrisceptreBullet(void)
{
    // Delete the sprite
    delete this->m_Sprite;

    // Delete physics stuff
    cpSpace *space = this->m_Engine->GetWorld()->GetSpace();
    cpSpaceRemoveShape(space, this->m_Shape);
    cpSpaceRemoveBody(space, this->m_Body);
    cpShapeFree(this->m_Shape);
    cpBodyFree(this->m_Body);
}

void TrisceptreBullet::HandleDamage(Player *player)
{
    player->DecreaseHealth(.1f);
}

void TrisceptreBullet::Initialize(void)
{
    // Load the texture
    if (!(this->m_Texture = this->m_Hge->Texture_Load("data/particles.png")))
	{
		MessageBox(NULL, "Can't load particles.png texture.", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		this->m_Engine->Shutdown();
		
        return;
	}

    // Get the color of the owner
    DWORD color = this->m_Owner->GetColor();

    // Create the player sprite
    this->m_Sprite = new hgeSprite(this->m_Texture, 20, 20, 20, 20);
    this->m_Sprite->SetColor(color);
	this->m_Sprite->SetHotSpot(10, 10);

    // Construct the physics stuff
    {
        // Define moment
        cpFloat moment = cpMomentForCircle(1, 0, 20, cpvzero);

        // Fetch the physics space
        cpSpace *space = this->m_Engine->GetWorld()->GetSpace();

        // Add the physics body
        this->m_Body = cpSpaceAddBody(space, cpBodyNew(1, moment));
        cpBodySetPos(this->m_Body, *this->m_Position);

        // Create and add the poly shape
        this->m_Shape = cpSpaceAddShape(space, cpCircleShapeNew(this->m_Body, 5, cpvzero));   
        this->m_Shape->sensor         = true;
        this->m_Shape->data           = this;
        this->m_Shape->collision_type = COLLISION_TYPE_BULLET;
    }
    
    // Intialize the base
    this->InitializeBullet();
}

void TrisceptreBullet::Render(float dt)
{
    // Render the bullet
    this->m_Sprite->Render(this->m_Position->x, this->m_Position->y);
}

void TrisceptreBullet::Update(float dt)
{
    // Fix the velocity
    this->m_Body->v = cpvmult(this->m_Body->v, this->m_Speed / cpvdist(cpVect(), this->m_Body->v));

    // Steer the bullet in the right direction
    this->m_Body->v.x = this->m_DirectionVector.x * 2.2f;

    // Update the position
    this->m_Position = new cpVect(cpBodyGetPos(this->m_Body));
}