#include "StdAfx.h"
#include "OUMapItemBuilding.h"
#include "OUMapResourceBuilding.h"

OUMapItemBuilding::OUMapItemBuilding(void) :
    m_pCover(NULL)
{
    m_emClassType = OUMIT_BUILDING;
}

OUMapItemBuilding::~OUMapItemBuilding(void)
{
    SAFEDEL(m_pSprite);
    SAFEDEL(m_pCover);
}

bool OUMapItemBuilding::SetBuildingInfo(OUMapBuildingInfo &info)
{
    SAFEDEL(m_pSprite);

    m_fWorldX = info.m_fTrueX;
    m_fWorldY = info.m_fTrueY;
    m_nOrder = info.m_Order;

    OUMapResourceBuilding* res = new OUMapResourceBuilding();
    if(!res->Load(info.m_dwID))
    {
        SAFEDEL(res);
        m_pSprite = NULL;

        return false;
    }

    m_pSprite = (OUMapResourceObject*)res;

    m_fWidth = m_pSprite->GetWidth();
    m_fHeight = m_pSprite->GetHeight();
    m_nResID = info.m_dwID;

    for(int i = 0; i < info.m_pCoverCoor->size(); i++)
    {
        m_CoverInfo.insert((*info.m_pCoverCoor)[i]);
    }

    m_pCover = new OUMapResourceCover();
    if(!m_pCover->Load(info.m_dwID))
    {
        SAFEDEL(m_pCover);
    }

    return true;
}

int OUMapItemBuilding::GetResourceID()
{
    return m_nResID;
}

void OUMapItemBuilding::Update(float fDelta)
{
    if(m_pSprite) m_pSprite->Update(fDelta);
}

void OUMapItemBuilding::Render(float fStartX, float fStartY)
{
    if(!m_pSprite) return;

    m_pSprite->Render(m_fWorldX - fStartX, m_fWorldY - fStartY);
}

void OUMapItemBuilding::SetZ(float z)
{
    if(!m_pSprite) return;
    m_pSprite->SetZ(z);
}

float OUMapItemBuilding::GetZ()
{
    if(!m_pSprite) return 1.0f;
    return m_pSprite->GetZ();
}

void OUMapItemBuilding::RenderCover(float fStartX, float fStartY)
{
    if(!m_pCover) return;

    m_pCover->Render(m_fWorldX - fStartX, m_fWorldY - fStartY);
}
