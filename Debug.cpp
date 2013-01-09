#include "StdAfx.h"

#include "Debug.h"

void Debug::InitDebug(void)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}