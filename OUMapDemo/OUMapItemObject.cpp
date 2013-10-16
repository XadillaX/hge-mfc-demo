#include "StdAfx.h"
#include "OUMapItemObject.h"

OUMapItemObject::OUMapItemObject(void) :
    m_emClassType(OUMIT_NONE),
    m_pSprite(NULL),
    m_fWorldX(0.0f),
    m_fWorldY(0.0f),
    m_fWidth(0.0f),
    m_fHeight(0.0f),

    m_pHGE(NULL)
{
    m_pHGE = hgeCreate(HGE_VERSION);
}

OUMapItemObject::~OUMapItemObject(void)
{
    if(m_pHGE)
    {
        m_pHGE->Release();
        m_pHGE = NULL;
    }
}

bool OUMapItemObject::IsInsideScreen(float fStartX, float fStartY, float fScreenWidth, float fScreenHeight)
{
    hgeRect rect1(fStartX, fStartY, fStartX + fScreenWidth, fStartY + fScreenHeight);
    hgeRect rect2(m_fWorldX, m_fWorldY, m_fWorldX + m_fWidth, m_fWorldY + m_fHeight);

    return rect1.Intersect(&rect2);
}
