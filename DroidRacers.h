/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <string>

class World;
class ServerConnection;

class Engine
{
private:
    static Engine *s_Singleton;

    ServerConnection *m_ServerConnection;

    bool         m_Booted;
    bool         m_IsShutdown;

    HGE         *m_Hge;

    World       *m_World;

    HSTREAM      m_MainMusic;

    std::string  m_LogFile;
    std::string  m_Title;
    bool         m_Windowed;
    unsigned int m_Fps;
    unsigned int m_Width;
    unsigned int m_Height;
    unsigned int m_Bpp;

    bool         m_DrawStats;

    hgeFont     *m_DefaultFont;

    Engine(void);
public:
    static Engine *GetSingleton(void);

    ~Engine(void);

    bool GfxRestoreFunc(void);
    bool FrameFunc(void);
    bool RenderFunc(void);

    // Some additional rendering functions
    void DrawLine(hgeVector a, hgeVector b, DWORD color, float width = 1.0f, int blend = BLEND_DEFAULT);

    HGE *GetHge(void);

    World *GetWorld(void)
    {
        return this->m_World;
    }

    unsigned int GetWidth(void)
    {
        return this->m_Width;
    }
    unsigned int GetHeight(void)
    {
        return this->m_Height;
    }
    hgeFont *GetDefaultFont(void)
    {
        return this->m_DefaultFont;
    }

    void SetDrawStats(bool stats);
    void SetLogFile(std::string logFile);
    void SetTitle(std::string title);
    void SetWindowed(bool windowed);
    void SetFps(unsigned int fps);
    void SetWidth(unsigned int width);
    void SetHeight(unsigned int height);
    void SetBpp(unsigned int bpp);

    void Boot(void);
    void Shutdown(void);
};