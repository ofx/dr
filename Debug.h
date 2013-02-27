/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#ifdef _DEBUG
//#define RENDER_BOUNDARIES
#endif

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__) 
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

class Debug
{
public:
    static void InitDebug(void);
};