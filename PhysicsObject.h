/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#define COLLISION_TYPE_LEVEL     0
#define COLLISION_TYPE_GO        1
#define COLLISION_TYPE_ACTIVATOR 2

class PhysicsObject : public GameObject
{
public:
    virtual void Initialize(void) = 0;
    virtual void Render(float dt) = 0;
    virtual void Update(float dt) = 0;

    static int BeginCollisionD(cpArbiter *arb, struct cpSpace *space, void *data);
    static int PreCollisionD(cpArbiter *arb, struct cpSpace *space, void *data);
    static void PostCollisionD(cpArbiter *arb, struct cpSpace *space, void *data);
    static void SeparateCollisionD(cpArbiter *arb, struct cpSpace *space, void *data);

    virtual int BeginCollision(cpArbiter *arb, struct cpSpace *space, void *data) = 0;
    virtual int PreCollision(cpArbiter *arb, struct cpSpace *space, void *data) = 0;
    virtual void PostCollision(cpArbiter *arb, struct cpSpace *space, void *data) = 0;
    virtual void SeparateCollision(cpArbiter *arb, struct cpSpace *space, void *data) = 0;
};