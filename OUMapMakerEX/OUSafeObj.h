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
    OUSafeObj(const char* szName = "") { strcpy(m_szName, szName); ::InitializeCriticalSection(&m_CS); }
    OUSafeObj(T value, const char* szName = "")
    {
        strcpy(m_szName, szName);
        m_Value = value;
        ::InitializeCriticalSection(&m_CS);
        m_bLocked = false;
    }
    virtual ~OUSafeObj()
    {
        ::DeleteCriticalSection(&m_CS);
        m_bLocked = false;
    }

    void                        SetValue(T value)
    {
        if(m_szName[0]) TRACE("即将修改 %s。\n", m_szName);

        ::EnterCriticalSection(&m_CS);
        m_Value = value;
        ::LeaveCriticalSection(&m_CS);
    }

    T                           GetValue()
    {
        if(m_szName[0]) TRACE("即将获取 %s。\n", m_szName);
        T v;
        ::EnterCriticalSection(&m_CS);
        v = m_Value;
        ::LeaveCriticalSection(&m_CS);

        return v;
    }

    T                           GetValueDirectly()
    {
        if(m_szName[0]) TRACE("即将直接获取 %s。\n", m_szName);
        return m_Value;
    }

    void                        SetValueDirectly(T value)
    {
        if(m_szName[0]) TRACE("即将直接修改 %s。\n", m_szName);
        m_Value = value;
    }

    T operator = (T value)
    {
        SetValue(value);
        return GetValue();
    }

    T operator == (T value)
    {
        return GetValue() == value;
    }

    void Lock() { if(m_szName[0]) TRACE("即将手动加锁 %s。\n", m_szName); m_bLocked = true; ::EnterCriticalSection(&m_CS); }
    void Unlock() { if(!m_bLocked) return; if(m_szName[0]) TRACE("即将手动解锁 %s。\n", m_szName); ::LeaveCriticalSection(&m_CS); m_bLocked = false; }

private:
    T                           m_Value;
    ::CRITICAL_SECTION          m_CS;
    bool                        m_bLocked;

    char                        m_szName[32];
};

#endif
