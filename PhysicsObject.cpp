#include "StdAfx.h"

#include "PhysicsObject.h"

int PhysicsObject::BeginCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    try
    {
        PhysicsObject *obj = (PhysicsObject*) data;
        return obj->BeginCollision(arb, space, data);
    }
    catch (...)
    {
        return 0;
    }
}
int PhysicsObject::PreCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    try
    {
        PhysicsObject *obj = (PhysicsObject*) data;
        return obj->PreCollision(arb, space, data);
    }
    catch (...)
    {
        return 0;
    }
}
void PhysicsObject::PostCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    try
    {
        PhysicsObject *obj = (PhysicsObject*) data;
        obj->PostCollision(arb, space, data);
    }
    catch (...)
    {
        return;
    }
}
void PhysicsObject::SeparateCollisionD(cpArbiter *arb, struct cpSpace *space, void *data)
{
    // Get the collision shapes
    CP_ARBITER_GET_SHAPES(arb, a, b);

    try
    {
        PhysicsObject *obj = (PhysicsObject*) data;
        obj->SeparateCollision(arb, space, data);
    }
    catch (...)
    {
        return;
    }
}