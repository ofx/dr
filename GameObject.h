/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

class cpVect;

class Engine;

class GameObject
{
protected:
    cpVect m_Position;

    HGE    *m_Hge;

    Engine *m_Engine;
public:
    GameObject(void);
    virtual ~GameObject(void);

    cpVect GetPosition(void)
    {
        return this->m_Position;
    }

    virtual void Initialize(void) = 0;
    virtual void Render(float dt) = 0;
    virtual void Update(float dt) = 0;

    virtual void RenderText(hgeFont *font, float dt) {};
};

