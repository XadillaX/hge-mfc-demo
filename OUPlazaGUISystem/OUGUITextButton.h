/***********************************
 *
 * O游棋牌GUI系统
 * - 带文字按钮类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUITEXTBUTTON_H__
#define __OUGUITEXTBUTTON_H__
#pragma once
#include "ouguibutton.h"
#include "OUFont.h"

class OUGUI_API OUGUITextButton : public OUGUIButton
{
public:
    OUGUITextButton(void);
    virtual ~OUGUITextButton(void);

    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);

    virtual void                OnRender();

    void                        SetFontname(string szFontname) { m_szFontname = szFontname; }
    string                      GetFontname() { return m_szFontname; }

    OUFont*                     GetFont() { return OUFontLoader::instance().GetFont(m_szFontname); }

    const char*                 GetText() { return m_szText.c_str(); }
    void                        SetText(const char* szText) { m_szText = szText; }

    void                        SetColor(OUBUTTON_STATE state, DWORD color)
    {
        m_dwColor[state] = color;
    }

    void                        SetColor(OUBUTTON_STATE state, const char* color)
    {
        DWORD clr;
        sscanf(color, "%X", &clr);
        m_dwColor[state] = clr;
    }

    DWORD                       GetColor(OUBUTTON_STATE state)
    {
        return m_dwColor[state];
    }

protected:
    string                      m_szFontname;
    string                      m_szText;

    DWORD                       m_dwColor[4];
};

#endif
