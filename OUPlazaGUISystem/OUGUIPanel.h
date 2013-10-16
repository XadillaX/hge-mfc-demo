/***********************************
 *
 * O游棋牌GUI系统
 * - 静态面板GUI -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUIPANEL_H__
#define __OUGUIPANEL_H__
#pragma once
#include "ouguiobject.h"

class OUGUI_API OUGUIPanel : public OUGUIObject
{
public:
    OUGUIPanel(void);
    virtual ~OUGUIPanel(void);

    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);
    virtual void                OnRender();
    virtual void                OnUpdate(float fDelta, float fMX, float fMY) { m_Sprite.Update(fDelta); }

    virtual void                OnMouseEnter() { m_dwHitColor = 0xffff0000; }
    virtual void                OnMouseLeave() { m_dwHitColor = 0xffffffff; }
    virtual void                OnMouseLButtonDown() { m_dwHitColor = 0xff00ff00; }
    virtual void                OnMouseLButtonUp() { if(GetMouseEnter()) m_dwHitColor = 0xffff0000; else m_dwHitColor = 0xffffffff; }

    virtual bool                OnCheckCollision(float fMX, float fMY);

protected:
    HTEXTURE                    m_hTex;
    //hgeSprite*                  m_pSprite;
    OUGUI_IMG                   m_Sprite;

    DWORD                       m_dwHitColor;
};

#endif
