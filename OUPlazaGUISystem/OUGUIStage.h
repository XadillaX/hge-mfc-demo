/***********************************
 *
 * O游棋牌GUI系统
 * - GUI游戏舞台抽象 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUISTAGE_H__
#define __OUGUISTATE_H__
#pragma once
#include "ouguiobject.h"

class OUGUIStage : public OUGUIObject
{
public:
    OUGUIStage(void);
    virtual ~OUGUIStage(void);

    virtual bool OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
    {
        if(OUGUIObject::OnLoadFromXml(pElement, pWillParent))
        {
            m_bInited = true;
            return true;
        }
        else return false;
    }

    virtual void OnRender()
    {
        //OUPOINT ap = this->GetAbsolutePos();
        //m_pHGE->Gfx_RenderLine(ap.m_fX, ap.m_fY, ap.m_fX + m_ptSize.m_fX, ap.m_fY + m_ptSize.m_fY, 0xffff0000);
        //m_pHGE->Gfx_RenderLine(ap.m_fX + m_ptSize.m_fX, ap.m_fY, ap.m_fX, ap.m_fY + m_ptSize.m_fY, 0xffff0000);
    }
};

#endif

