#include "StdAfx.h"
#include "OUGUITextButton.h"

REGISTERGUI("textbutton", OUGUITextButton, __ou_gui_textbutton_creator);

OUGUITextButton::OUGUITextButton(void) :
    m_szFontname("_default"),
    m_szText("")
{
    memset(m_dwColor, 0, sizeof(m_dwColor));
}

OUGUITextButton::~OUGUITextButton(void)
{
}

void OUGUITextButton::OnRender()
{
    OUGUIButton::OnRender();

    OUPOINT ap = GetAbsolutePos();
    OUPOINT sz = GetSize();
    OUFont* font = GetFont();
    font->SetColor(m_dwColor[m_State]);

    SIZE fsz = font->GetTextSize(C2W(GetText()));
    
    ap.m_fX += ((sz.m_fX - fsz.cx) / 2);
    ap.m_fY += ((sz.m_fY - font->GetFontSize()) / 2);

    font->Print(ap.m_fX, ap.m_fY, "%s", m_szText.c_str());
}

bool OUGUITextButton::OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
{
    TiXmlElement* pNode;
    for(pNode = pElement->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
    {
        Assert(pNode->Value(), "Empty node value.");
        OUStringFunc str = pNode->Value();
        str.ToUppercase();

        if(str.GetString() == "FONT")
        {
            SetFontname(pNode->GetText());
        }
        else
        if(str.GetString() == "TEXT")
        {
            SetText(pNode->GetText());
        }
        else
        if(str.GetString() == "COLORUP")
        {
            SetColor(OUBS_UP, pNode->GetText());
        }
        else
        if(str.GetString() == "COLORDOWN")
        {
            SetColor(OUBS_DOWN, pNode->GetText());
        }
        else
        if(str.GetString() == "COLORHOVER")
        {
            SetColor(OUBS_HOVER, pNode->GetText());
        }
        else
        if(str.GetString() == "COLORDISABLE")
        {
            SetColor(OUBS_DISABLE, pNode->GetText());
        }
    }

    return OUGUIButton::OnLoadFromXml(pElement, pWillParent);
}
