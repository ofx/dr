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

    float m_Health;

    float m_Speed;

    float m_DeltaX;
    float m_DeltaY;

    float m_SteeringValue;

    unsigned int m_PlayerIndex;

    const float SPEED;
    const float FRICTION;

    void TransferWeapon(Weapon* weapon);
    void Shoot(void);
    void Boost(void);
    void EndBoost(void);
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
    cpVect GetDirectionVector(void)
    {
        return this->m_Body->v;
    }

    void DecreaseHealth(float value);
    void IncreaseHealth(float value);

    int BeginCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    int PreCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    void PostCollision(cpArbiter *arb, struct cpSpace *space, void *data);
    void SeparateCollision(cpArbiter *arb, struct cpSpace *space, void *data);

    void SetSpeed(float speed);
    void SetSteeringValue(float steeringValue);

    void CycleWeapons(void);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);

    friend class PlayerManager;
    friend struct PlayerSort;
};

