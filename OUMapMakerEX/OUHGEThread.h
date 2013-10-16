#ifndef __OUHGETHREAD_H__
#define __OUHGETHREAD_H__

extern DWORD WINAPI __HGETHREAD(LPVOID param);
extern bool __HGEUpdate();
extern bool __HGERender();
extern void __HGETimerFunc(LPVOID param);

extern ::CRITICAL_SECTION g_csRenderLock;

#endif
