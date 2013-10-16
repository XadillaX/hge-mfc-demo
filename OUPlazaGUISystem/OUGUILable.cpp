#include "StdAfx.h"
#include "OUGUILable.h"

REGISTERGUI("lable", OUGUILable, __ou_gui_lable_creator);

OUGUILable::OUGUILable(void) :
    m_szFontname(""),
    m_szText(""),
    m_dwColor(0)
{
}

OUGUILable::~OUGUILable(void)
{
}

void OUGUILable::OnRender()
{
    OUPOINT ap = this->GetAbsolutePos();
    
    OUFont* font = GetFont();
    font->SetColor(GetColor());
    font->Print(ap.m_fX, ap.m_fY, "%s", m_szText.c_str());

    //this->OnDrawBoard(0xffff0000);
}

bool OUGUILable::OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
{
    OUGUIObject::OnLoadFromXml(pElement, pWillParent);

    TiXmlElement* pNode;
    for(pNode = pElement->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
    {
        Assert(pNode->Value(), "Empty node value.");
        OUStringFunc str = pNode->Value();
        str.ToUppercase();

        if(str.GetString() == "TOP")
        {
            m_ptPos.m_fY = atoi(pNode->GetText());
        }
        else
        if(str.GetString() == "LEFT")
        {
            m_ptPos.m_fX = atoi(pNode->GetText());
        }
        else
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
        if(str.GetString() == "COLOR")
        {
            SetColor(pNode->GetText());
        }
    }

    m_bInited = true;
    return true;
}
