/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "Player.h"

Player::Player(cpVect position, DWORD color, int index, std::string name) : SPEED(3.5f), FRICTION(0.98f)
{
    this->m_Color = color;
    this->m_PlayerIndex = index;
    this->m_Name = name;

    this->m_Position = new cpVect(position);

    this->m_DeltaX = 0.0f;
    this->m_DeltaY = 0.0f;

    this->m_Speed = 0.9;

    // Initialize the player
    this->Initialize();
}

Player::~Player(void)
{
    delete this->m_ParticleSystem;
	delete this->m_Sprite;
	delete this->m_ParticleSprite;

    // Delete physics stuff
    cpSpace *space = this->m_Engine->GetWorld()->GetSpace();
    cpSpaceRemoveShape(space, this->m_Shape);
    cpSpaceRemoveBody(space, this->m_Body);
    cpShapeFree(this->m_Shape);
    cpBodyFree(this->m_Body);
}

void Player::Initialize(void)
{
    // Load the texture
    if (!(this->m_Texture = this->m_Hge->Texture_Load("data/particles.png")))
	{
		MessageBox(NULL, "Can't load particles.png texture.", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		this->m_Engine->Shutdown();
		
        return;
	}

    // Create the player sprite
    this->m_Sprite = new hgeSprite(this->m_Texture, 60, 40, 20, 20);
    this->m_Sprite->SetColor(this->m_Color);
	this->m_Sprite->SetHotSpot(10, 10);

    // Create the particle sprite
	this->m_ParticleSprite = new hgeSprite(this->m_Texture, 20, 20, 20, 20);
	this->m_ParticleSprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
	this->m_ParticleSprite->SetHotSpot(10, 10);

    // Create the particle system
	this->m_ParticleSystem = new hgeParticleSystem("data/trail.psi", this->m_ParticleSprite);
	this->m_ParticleSystem->Fire();

    // Initialize physics
    {
        // Define moment
        cpFloat moment = cpMomentForCircle(1, 0, 10, cpvzero);

        // Fetch the physics space
        cpSpace *space = this->m_Engine->GetWorld()->GetSpace();

        // Add the physics body
        this->m_Body = cpSpaceAddBody(space, cpBodyNew(1, moment));
        cpBodySetPos(this->m_Body, *this->m_Position);
        
        // Add the physics shape
        this->m_Shape = cpSpaceAddShape(space, cpCircleShapeNew(this->m_Body, 10, cpvzero));
        cpShapeSetFriction(this->m_Shape, 0.7);
    }
}

void Player::SetSpeed(float speed)
{
    this->m_Speed = speed;
}

void Player::Render(float dt)
{
	this->m_ParticleSystem->Render();
	this->m_Sprite->Render(this->m_Position->x, this->m_Position->y);

    // Set the world camera to follow the player
    // TODO: Follow player group
    this->m_Engine->GetWorld()->WorldCamera->Position.x = (this->m_Engine->GetWidth() * 0.5f) - this->m_Position->x;
    this->m_Engine->GetWorld()->WorldCamera->Position.y = (this->m_Engine->GetHeight() * 0.5f) - this->m_Position->y;
}

inline bool Player::operator>(const Player &other) const 
{ 
    return this->m_Position->y < other.m_Position->y;
}

void Player::Update(float dt)
{
    // Fix the velocity
    this->m_Body->v = cpvmult(this->m_Body->v, this->m_Speed / cpvdist(cpVect(), this->m_Body->v));

    // Update the position
    this->m_Position = new cpVect(cpBodyGetPos(this->m_Body));

#ifdef _DEBUG
        int KEY_MAP[5][2] = {
            {HGEK_LEFT, HGEK_RIGHT},
            {HGEK_A,    HGEK_S},
            {HGEK_R,    HGEK_T},
            {HGEK_V,    HGEK_B},
            {HGEK_U,    HGEK_I}
        };

        // Process keys
        if (this->m_Hge->Input_GetKeyState(HGEK_ESCAPE)) 
        {
            this->m_Engine->Shutdown();
        }
        if (this->m_Hge->Input_GetKeyState(KEY_MAP[this->m_PlayerIndex][0])) 
        {
            this->m_Body->v.x -= SPEED * dt;
        }
        if (this->m_Hge->Input_GetKeyState(KEY_MAP[this->m_PlayerIndex][1]))
        {
            this->m_Body->v.x += SPEED * dt;
        }
#endif

	// Do some movement calculations and collision detection	
	this->m_Body->v.x *= FRICTION;
    this->m_Body->v.y *= FRICTION;

	// Update particle system
    float a = abs(this->m_Body->v.x) * 50.0f;
    float b = abs(this->m_Body->v.y) * 50.0f;
	this->m_ParticleSystem->info.nEmission = (int) (a * a + b * b);
    this->m_ParticleSystem->MoveTo(this->m_Position->x, this->m_Position->y);
	this->m_ParticleSystem->Update(dt);
}