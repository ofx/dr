/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

class Trisceptre : public Weapon
{
public:
    Trisceptre(Player *owner, World *world);
    ~Trisceptre(void);

    void Shoot(void);
    static void ShootP(cpSpace *space, void *obj, void *data);
};