/***********************************
 *
 * O游棋牌GUI系统
 * - 标签GUI类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUILABLE_H__
#define __OUGUILABLE_H__
#pragma once
#include "ouguiobject.h"

class OUGUI_API OUGUILable : public OUGUIObject
{
public:
    OUGUILable(void);
    virtual ~OUGUILable(void);

    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);
    virtual void                OnRender();

    void                        SetFontname(string szFontname) { m_szFontname = szFontname; ChgResize(); }
    string                      GetFontname() { return m_szFontname; }

    OUFont*                     GetFont() { return OUFontLoader::instance().GetFont(m_szFontname); }

    const char*                 GetText() { return m_szText.c_str(); }
    void                        SetText(const char* szText)
    {
        m_szText = szText;
        ChgResize();
    }

    void                        SetColor(DWORD color)
    {
        m_dwColor = color;
    }

    void                        SetColor(const char* color)
    {
        DWORD clr;
        sscanf(color, "%X", &clr);
        m_dwColor = clr;
    }

    DWORD                       GetColor()
    {
        return m_dwColor;
    }

    void                        ChgResize()
    {
        SIZE sz = GetFont()->GetTextSize(C2W(GetText()));
        m_ptSize.m_fX = sz.cx;
        m_ptSize.m_fY = sz.cy;
    }

protected:
    string                      m_szFontname;
    string                      m_szText;

    DWORD                       m_dwColor;
};

#endif
