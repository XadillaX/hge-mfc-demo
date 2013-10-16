#include "StdAfx.h"
#include "OUMapResourceObject.h"

OUMapResourceObject::OUMapResourceObject(void) :
    m_dwID(0)
{
    m_pHGE = hgeCreate(HGE_VERSION);
    VERIFY(m_pHGE);
}

OUMapResourceObject::~OUMapResourceObject(void)
{
    if(m_pHGE)
    {
        m_pHGE->Release();
        m_pHGE = NULL;
    }
}
