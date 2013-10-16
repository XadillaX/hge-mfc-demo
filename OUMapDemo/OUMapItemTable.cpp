#include "StdAfx.h"
#include "OUMapItemTable.h"

OUMapItemTable::OUMapItemTable(void) :
    m_nTableID(0)
{
    m_emClassType = OUMIT_TABLE;
}

OUMapItemTable::~OUMapItemTable(void)
{
    SAFEDEL(m_pSprite);
}

bool OUMapItemTable::SetBuildingInfo(OUMapBuildingInfo& info)
{
    SAFEDEL(m_pSprite);
    if(!info.m_pExtendInfo) return false;
    OUMapTableInfo* tableinfo = (OUMapTableInfo*)info.m_pExtendInfo;

    m_fWorldX = info.m_fTrueX;
    m_fWorldY = info.m_fTrueY;
    m_nOrder = info.m_Order;

    OUMapResourceTable* res = new OUMapResourceTable();
    if(!res->Load(tableinfo->m_nImgID))
    {
        SAFEDEL(res);
        m_pSprite = NULL;

        return false;
    }

    m_pSprite = res;
    m_nTableID = tableinfo->m_nID;
    m_fWidth = m_pSprite->GetWidth();
    m_fHeight = m_pSprite->GetHeight();
    m_nResID = tableinfo->m_nImgID;

    return true;
}

void OUMapItemTable::Render(float fStartX, float fStartY)
{
    if(!m_pSprite) return;

    ((OUMapResourceTable*)m_pSprite)->RenderTable(m_fWorldX - fStartX, m_fWorldY - fStartY, m_nTableID);
}

void OUMapItemTable::ResponseInput(float itemX, float itemY)
{
    if(!m_pSprite) return;

    OUMapResourceTable* table = (OUMapResourceTable*)m_pSprite;

    table->ResumeSeats();
    int seatIdx = table->GetSeatByMouse(itemX, itemY);
    if(OUMapResourceTableItem::LAYER_NOPOS != seatIdx)
    {
        table->SetColor(seatIdx, 0xffcccccc);
    }
}
