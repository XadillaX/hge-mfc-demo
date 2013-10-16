/***********************************
 *
 * O游棋牌GUI系统
 * - 垂直滚动条GUI类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUIVSCROLLBAR_H__
#define __OUGUIVSCROLLBAR_H__
#pragma once
#include "global.h"
#include "ouguiobject.h"
/** TODO */
class OUGUIVScrollBar : public OUGUIObject
{
public:
    OUGUIVScrollBar(void);
    virtual ~OUGUIVScrollBar(void);

protected:
    OUGUI_IMG               m_Background;
    OUGUI_IMG               m_Bar;
};

#endif
