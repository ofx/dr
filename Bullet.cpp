#include "StdAfx.h"

#include "Bullet.h"

Bullet::Bullet(Player *owner, cpVect directionVector)
{
    this->m_Owner           = owner;
    this->m_Position        = owner->GetPosition();
    this->m_DirectionVector = directionVector;
    this->m_LevelCollisionCount = 0;
    this->m_Age                 = 0.0f;
}

Bullet::~Bullet(void)
{
}

void Bullet::InitializeBullet(void)
{
    // Retrieve the space
    cpSpace *space = this->m_Engine->GetWorld()->GetSpace();

    // Add collision handler for collisions with level geometry
    cpSpaceAddCollisionHandler(space, COLLISION_TYPE_BULLET, COLLISION_TYPE_LEVEL, BeginCollisionD, NULL, NULL, NULL, this);

    // Set active
    this->m_Active = true;
}

void Bullet::Update(float dt)
{
    this->m_Age += dt;

    if (this->m_Age >= MAX_BULLET_AGE)
    {
        // Make inactive
        this->m_Active = false;

        // Collision with the level, kill bullet
        this->m_Engine->GetWorld()->RemoveGameObject(this);
    }
}

int Bullet::BeginCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
    if (!this->m_Active)
    {
        return false;
    }

    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    // Delimit collision types for both shapes
    if (a->collision_type == COLLISION_TYPE_BULLET && b->collision_type == COLLISION_TYPE_LEVEL)
    {
        // Make inactive
        this->m_Active = false;

        // Collision with the level, kill bullet
        this->m_Engine->GetWorld()->RemoveGameObject(this);
    }

    return false;
}

int Bullet::PreCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
    return true;
}

void Bullet::PostCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
}

void Bullet::SeparateCollision(cpArbiter *arb, struct cpSpace *space, void *data)
{
}