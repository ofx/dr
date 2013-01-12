/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#define NUM_WEAPON_SLOTS 2

class cpBody;
class cpShape;

class Player : public PhysicsObject
{
private:
    HTEXTURE m_Texture;

    hgeSprite *m_Sprite;
    hgeSprite *m_ParticleSprite;
    
    hgeParticleSystem *m_ParticleSystem;

    std::string m_Name;

    cpBody  *m_Body;
    cpShape *m_Shape;

    DWORD m_Color;

    Weapon **m_Weaponslots;
    int m_ActiveWeaponslot;

    float m_Speed;

    float m_DeltaX;
    float m_DeltaY;

    unsigned int m_PlayerIndex;

    const float SPEED;
    const float FRICTION;
public:
    Player(cpVect position, DWORD color, int index, std::string name);
    ~Player(void);

    std::string GetName(void)
    {
        return this->m_Name;
    }
    DWORD GetColor(void)
    {
        return this->m_Color;
    }

    int BeginCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    int PreCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    void PostCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    void SeparateCollision(cpArbiter *arb, struct cpSpace *space, void *data);

    void SetSpeed(float speed);

    // Provide an interface to sort the ranking of players
    bool operator>(const Player &player) const;

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);

    friend class PlayerManager;
};

