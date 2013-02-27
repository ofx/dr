/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "Player.h"

Player::Player(cpVect position, DWORD color, int index, std::string name) : SPEED(40.0f), FRICTION(0.98f)
{
    this->m_Active = true;

    this->m_Color = color;
    this->m_PlayerIndex = index;
    this->m_Name = name;

    this->m_SteeringValue = 0.0f;

    this->m_ActiveWeaponslot = 0;

    this->m_Position = cpVect(position);

    this->m_Health = 1.0f;

    this->m_DeltaX = 0.0f;
    this->m_DeltaY = 0.0f;

    this->m_Speed = 0.9f;

    // Initialize the player
    this->Initialize();
}

Player::~Player(void)
{
    delete this->m_ParticleSystem;
	delete this->m_Sprite;
	delete this->m_ParticleSprite;

    // Delete weapon in weapon slots
    for (int i = 0 ; i < NUM_WEAPON_SLOTS ; ++i)
    {
        delete this->m_Weaponslots[i];
    }
    delete this->m_Weaponslots;

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

    // Initialize the weapon slots
    this->m_Weaponslots = (Weapon**) malloc(sizeof(Weapon*) * NUM_WEAPON_SLOTS);
    for (int i = 0 ; i < NUM_WEAPON_SLOTS ; ++i)
    {
        this->m_Weaponslots[i] = 0;
    }
    this->m_Weaponslots[0] = new Neoshooter(this, this->m_Engine->GetWorld());

    // Initialize physics
    {
        // Define moment
        cpFloat moment = cpMomentForCircle(1, 0, 10, cpvzero);

        // Fetch the physics space
        cpSpace *space = this->m_Engine->GetWorld()->GetSpace();

        // Add the physics body
        this->m_Body = cpSpaceAddBody(space, cpBodyNew(1, moment));
        cpBodySetPos(this->m_Body, this->m_Position);

        // Add the physics shape
        this->m_Shape                 = cpSpaceAddShape(space, cpCircleShapeNew(this->m_Body, 10, cpvzero));
        this->m_Shape->data           = this;
        this->m_Shape->collision_type = COLLISION_TYPE_GO;
        cpShapeSetElasticity(this->m_Shape, 1.0f);
        cpShapeSetFriction(this->m_Shape, 0.7);

        // Define the collision handlers for various types of collisions
        cpSpaceAddCollisionHandler(space, COLLISION_TYPE_GO, COLLISION_TYPE_ACTIVATOR, BeginCollisionD, NULL, NULL, SeparateCollisionD, this);
        cpSpaceAddCollisionHandler(space, COLLISION_TYPE_GO, COLLISION_TYPE_BULLET, BeginCollisionD, NULL, NULL, NULL, this);
    }
}

void Player::TransferWeapon(Weapon *weapon)
{
    // Seek for an empty slot
    for (int i = 0 ; i < NUM_WEAPON_SLOTS ; ++i)
    {
        if (this->m_Weaponslots[i] == 0)
        {
            this->m_ActiveWeaponslot = i;
        }
    }

    // Delete the current weapon
    delete this->m_Weaponslots[this->m_ActiveWeaponslot];

    // Transfer the weapon from the activator to the player
    this->m_Weaponslots[this->m_ActiveWeaponslot] = weapon;
}

void Player::Shoot(void)
{
    // Shoot the active weapon
    this->m_Weaponslots[this->m_ActiveWeaponslot]->Shoot();
}

void Player::Boost(void)
{
    // Increase speed
    this->m_Speed = 1.3f;
}

void Player::EndBoost(void)
{
    // Default speed
    this->m_Speed = 0.9f;
}

int Player::BeginCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    // Delimit collision types for both shapes
    if (a->collision_type == COLLISION_TYPE_GO && b->collision_type == COLLISION_TYPE_ACTIVATOR)
    {
        // Retrieve the activator object
        Activator *activator = (Activator*) b->data;

        // Retrieve the player object
        Player *player = (Player*) a->data;
    
        // Check if the right player is triggering
        if (player == activator->Owner)
        {
            // Delegate
            switch(activator->ActivatorType)
            {
                case ACTIVATOR_TYPE_FIRE:
                    activator->Owner->Shoot();
                    break;
                case ACTIVATOR_TYPE_BOOST:
                    activator->Owner->Boost();
                    break;
                case ACTIVATOR_TYPE_WEAPON:
                    activator->Owner->TransferWeapon((Weapon*) activator->Data);
                    activator->Data = 0;
                    break;
            }
        }
        else
        {   
            // No need to continue
            return false;
        }
    }
    else if (a->collision_type == COLLISION_TYPE_GO && b->collision_type == COLLISION_TYPE_BULLET)
    {
        // Retrieve the associated objects
        Player *player = (Player*) a->data;
        Bullet *bullet = (Bullet*) b->data;

        // Check if it's not own bullet
        if (player != bullet->GetOwner())
        {
            bullet->HandleDamage(player);
        }
    }

    return true;
}

int Player::PreCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
    return true;
}

void Player::PostCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
}

void Player::SeparateCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    // Delimit collision types for both shapes
    if (a->collision_type == COLLISION_TYPE_GO && b->collision_type == COLLISION_TYPE_ACTIVATOR)
    {
        // Retrieve the activator object
        Activator *activator = (Activator*) b->data;

        // Retrieve the player object
        Player *player = (Player*) a->data;

        // Check if the right one is triggering
        if (player == activator->Owner)
        {
            // Delegate
            switch(activator->ActivatorType)
            {
                case ACTIVATOR_TYPE_BOOST:
                    this->EndBoost();
                    break;
            }
        }
    }
}

void Player::SetSpeed(float speed)
{
    this->m_Speed = speed;
}

void Player::SetSteeringValue(float steeringValue)
{
    this->m_SteeringValue = steeringValue;
}

void Player::DecreaseHealth(float value)
{
    this->m_Health -= value;
}

void Player::IncreaseHealth(float value)
{
    this->m_Health += value;
}

void Player::Render(float dt)
{
	this->m_ParticleSystem->Render();
	this->m_Sprite->Render(this->m_Position.x, this->m_Position.y);

    // Render the player's health
    this->m_Engine->GetDefaultFont()->printf(
        this->m_Position.x + 5.0f,
        this->m_Position.y + 5.0f,
        HGETEXT_LEFT,
        "%1.1f",
        this->m_Health
    );
}

void Player::CycleWeapons(void)
{
    if (this->m_ActiveWeaponslot + 1 >= NUM_WEAPON_SLOTS)
    {
        this->m_ActiveWeaponslot = 0;
    }
    else
    {
        if (this->m_Weaponslots[this->m_ActiveWeaponslot + 1] != 0)
        {
            ++this->m_ActiveWeaponslot;
        }
    }
}

void Player::Update(float dt)
{
    // Fix the velocity
    this->m_Body->v = cpvmult(this->m_Body->v, this->m_Speed / cpvdist(cpVect(), this->m_Body->v));

    // Update the position
    cpVect bv(cpBodyGetPos(this->m_Body));
    this->m_Position.x = bv.x;
    this->m_Position.y = bv.y;

    // Set the steering value
    this->m_Body->v.x = -this->m_SteeringValue * 5.0f * dt;

//#ifdef true
        int KEY_MAP[5][3] = {
            {HGEK_LEFT, HGEK_RIGHT, HGEK_O},
            {HGEK_A,    HGEK_S,     HGEK_D}
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
        if (this->m_Hge->Input_GetKeyState(KEY_MAP[this->m_PlayerIndex][2]))
        {
            this->CycleWeapons();
        }
//#endif

	// Do some movement calculations and collision detection	
	this->m_Body->v.x *= FRICTION;
    this->m_Body->v.y *= FRICTION;

	// Update particle system
    float a = abs(this->m_Body->v.x) * 50.0f;
    float b = abs(this->m_Body->v.y) * 50.0f;
	this->m_ParticleSystem->info.nEmission = (int) (a * a + b * b);
    this->m_ParticleSystem->MoveTo(this->m_Position.x, this->m_Position.y);
	this->m_ParticleSystem->Update(dt);
}