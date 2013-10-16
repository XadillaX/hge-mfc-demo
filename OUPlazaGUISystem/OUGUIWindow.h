/***********************************
 *
 * O游棋牌GUI系统
 * - 窗口GUI -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUIWINDOW_H__
#define __OUGUIWINDOW_H__
#pragma once
#include "OUGUIPanel.h"

class OUGUI_API OUGUIWindow : public OUGUIPanel
{
public:
    OUGUIWindow(void);
    virtual ~OUGUIWindow(void);

    /** 移动窗口 */
    virtual void                OnMouseMove(float fMX, float fMY)
    {
        if(!m_pHGE->Input_GetKeyState(HGEK_LBUTTON)) return;
        
        OUPOINT oldpos = GetRelativePos();
        OUPOINT oldmouse = GetRelativeMousePos();
        OUPOINT newmouse = { fMX, fMY };
        OUPOINT dpos = newmouse - oldmouse;
        OUPOINT newpos = oldpos + dpos;

        this->SetRelativePos(newpos);
    }
};
#endif
