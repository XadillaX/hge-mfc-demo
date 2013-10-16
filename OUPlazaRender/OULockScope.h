/***********************************
 *
 * O��������Ⱦϵͳ
 * - ���Զ��� -
 * ����: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OULOCKSCOPE_H__
#define __OULOCKSCOPE_H__
#pragma once
#include "hge.h"
#include <windows.h>

class OURENDER_EXPORT OULockScope
{
public:
    OULockScope(CRITICAL_SECTION &CS);
    virtual ~OULockScope();

    void                        Lock();
    void                        Unlock();

private:
    ::CRITICAL_SECTION          &m_CS;
    bool                        m_bLocked;
};
#endif
