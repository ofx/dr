/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

class cpBody;
class cpShape;

class Player : public GameObject
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

    void SetSpeed(float speed);

    // Provide an interface to sort the ranking of players
    bool operator>(const Player &player) const;

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);
};

