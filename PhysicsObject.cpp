#include "StdAfx.h"

#include "PhysicsObject.h"

int PhysicsObject::BeginCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    PhysicsObject *obj = (PhysicsObject*) data;
    return obj->BeginCollision(arb, space, data);
}
int PhysicsObject::PreCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    PhysicsObject *obj = (PhysicsObject*) data;
    return obj->PreCollision(arb, space, data);
}
void PhysicsObject::PostCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    PhysicsObject *obj = (PhysicsObject*) data;
    obj->PostCollision(arb, space, data);
}
void PhysicsObject::SeparateCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    PhysicsObject *obj = (PhysicsObject*) data;
    obj->SeparateCollision(arb, space, data);
}