/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

class Neoshooter : public Weapon
{
public:
    Neoshooter(Player *owner, World *world);
    ~Neoshooter(void);

    void Shoot(void);
    static void ShootP(cpSpace *space, void *obj, void *data);
};