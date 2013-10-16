/***********************************
 *
 * O游棋牌大厅世界
 * - 线程安全对象 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUSAFEOBJ_H__
#define __OUSAFEOBJ_H__
#pragma once
#include <windows.h>

template<class T>
class OUSafeObj
{
public:
    OUSafeObj() { ::InitializeCriticalSection(&m_CS); }
    OUSafeObj(T value)
    {
        m_Value = value;
        ::InitializeCriticalSection(&m_CS);
    }
    virtual ~OUSafeObj()
    {
        ::DeleteCriticalSection(&m_CS);
    }

    void                        SetValue(T value)
    {
        ::EnterCriticalSection(&m_CS);
        m_Value = value;
        ::LeaveCriticalSection(&m_CS);
    }

    T                           GetValue()
    {
        T v;
        ::EnterCriticalSection(&m_CS);
        v = m_Value;
        ::LeaveCriticalSection(&m_CS);

        return v;
    }

private:
    T                           m_Value;
    ::CRITICAL_SECTION          m_CS;
};

#endif
