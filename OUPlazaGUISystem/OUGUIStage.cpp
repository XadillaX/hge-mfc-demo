#include "StdAfx.h"
#include "OUGUIStage.h"

///////////////////////////////////////////////////////////
// 工厂对象

REGISTERGUI("stage", OUGUIStage, __ou_gui_stage_creator);

///////////////////////////////////////////////////////////

OUGUIStage::OUGUIStage(void)
{
    m_bStayBottom = true;
    m_ptPos.m_fX = m_ptPos.m_fY = 0;
    m_ptSize.m_fX = m_pHGE->System_GetState(HGE_SCREENWIDTH);
    m_ptSize.m_fY = m_pHGE->System_GetState(HGE_SCREENHEIGHT);
}

OUGUIStage::~OUGUIStage(void)
{
}
