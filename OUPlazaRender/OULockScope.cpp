#include "OULockScope.h"

OULockScope::OULockScope(CRITICAL_SECTION &CS) :
    m_CS(CS),
    m_bLocked(false)
{
    Lock();
}

OULockScope::~OULockScope(void)
{
    Unlock();
}

void OULockScope::Lock()
{
    if(!m_bLocked)
    {
        ::EnterCriticalSection(&m_CS);
        m_bLocked = true;
    }
}

void OULockScope::Unlock()
{
    if(m_bLocked)
    {
        ::LeaveCriticalSection(&m_CS);
        m_bLocked = false;
    }
}
