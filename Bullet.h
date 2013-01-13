#pragma once

#include "GameObject.h"

class Player;

class Bullet : public PhysicsObject
{
protected:
    Player *m_Owner;

    cpVect  m_DirectionVector;

    bool    m_Active;

    float   m_Speed;
public:
    Bullet(Player *owner, cpVect directionVector = cpVect());
    virtual ~Bullet(void);

    void InitializeBullet(void);

    int BeginCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    int PreCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    void PostCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    void SeparateCollision(cpArbiter *arb, struct cpSpace *space, void *data);

    Player *GetOwner(void)
    {
        return this->m_Owner;
    }
};

