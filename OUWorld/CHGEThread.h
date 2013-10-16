#ifndef __CHGETHREAD_H__
#define __CHGETHREAD_H__
#pragma once

#include "OURenderWorld.h"

extern OURenderWorld* g_pGameWorld;

extern DWORD WINAPI __HGEThread(LPVOID lpParam);
extern bool __HGEUpdate();
extern bool __HGERender();

#endif
