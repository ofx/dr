/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <list>

class GameObject;

typedef struct Camera
{
    cpVect Position;
    float Zoom;

    Camera(void)
    {
        this->Position.x = 0.0f;
        this->Position.y = 0.0f;
        this->Zoom = 0.0f;
    }
} Camera;

class World
{
private:
    Engine *m_Engine;

    cpSpace *m_Space;

    LevelGridManager *m_LevelManager;
    PlayerManager    *m_PlayerManager;

    std::list<GameObject*> m_DeletionList;
    std::list<GameObject*> m_Objects;
public:
    World(void);
    ~World(void);

    Camera *WorldCamera;

    cpSpace *GetSpace(void)
    {
        return this->m_Space;
    }
    PlayerManager *GetPlayerManager(void)
    {
        return this->m_PlayerManager;
    }

    void CameraTranspose(cpVect *pos);

    void InitializePhysics(void);
    void Initialize(void);

    void Render(float dt);
    void RenderText(hgeFont *font, float dt);
    void Update(float dt);

    void RemoveGameObject(GameObject *object);

    friend class LevelGridManager;
    friend class PlayerManager;
};