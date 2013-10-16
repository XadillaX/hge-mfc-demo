#include "StdAfx.h"
#include "OUGUIButton.h"

REGISTERGUI("button", OUGUIButton, __ou_gui_button_creator);

OUGUIButton::OUGUIButton(void) :
    m_hTex(0),

    m_szOnLButtonDown(""),
    m_szOnLButtonUp(""),
    m_State(OUBS_UP),
    m_OldState(OUBS_UP)
{
    memset(m_Sprite, 0, sizeof(m_Sprite));
}

OUGUIButton::~OUGUIButton(void)
{
    for(int i = 0; i < OUBS_MAX; i++)
    {
        if(m_Sprite[i].m_bStatic)
        {
            SAFEDEL(m_Sprite[i].m_Img.m_pSprite);
        }
        else
        {
            SAFEDEL(m_Sprite[i].m_Img.m_pAnim);
        }
    }

    if(0 != m_hTex) m_pHGE->Texture_Free(m_hTex);
}

void OUGUIButton::OnUpdate(float fDelta, float fMX, float fMY)
{
    /** Í£Ö¹±ðµÄ×´Ì¬ */
    for(int i = 0; i < OUBS_MAX; i++)
    {
        if(i != m_State && !m_Sprite[i].m_bStatic)
        {
            if(m_Sprite[i].m_Img.m_pAnim->GetAnimation()->IsPlaying())
            {
                m_Sprite[i].m_Img.m_pAnim->GetAnimation()->Stop();
            }
        }
    }

    /** ÈÃËüPlayÆðÀ´ */
    if(!m_Sprite[m_State].m_bStatic)
    {
        if(m_Sprite[m_State].m_Img.m_pAnim == NULL) return;
        if(!m_Sprite[m_State].m_Img.m_pAnim->GetAnimation()->IsPlaying())
        {
            m_Sprite[m_State].m_Img.m_pAnim->GetAnimation()->Play();
        }

        m_Sprite[m_State].m_Img.m_pAnim->GetAnimation()->Update(fDelta);
    }
}

void OUGUIButton::OnRender()
{
    OUPOINT ap = GetAbsolutePos();
    if(m_Sprite[m_State].m_bStatic && m_Sprite[m_State].m_Img.m_pSprite != NULL)
    {
        m_Sprite[m_State].m_Img.m_pSprite->Render(ap.m_fX, ap.m_fY);
    }
    else
    if(!m_Sprite[m_State].m_bStatic && m_Sprite[m_State].m_Img.m_pAnim != NULL)
    {
        m_Sprite[m_State].m_Img.m_pAnim->GetAnimation()->Render(ap.m_fX, ap.m_fY);
    }
}

bool OUGUIButton::OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
{
    OUGUIObject::OnLoadFromXml(pElement, pWillParent);

    TiXmlElement* pNode;
    for(pNode = pElement->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
    {
        Assert(pNode->Value(), "Empty node value.");
        OUStringFunc str = pNode->Value();
        str.ToUppercase();

        /** ×ó¼ü°´ÏÂ»Øµ÷ */
        if(str.GetString() == "LBUTTONDOWN")
        {
            m_szOnLButtonDown = pNode->GetText();
        }
        else
        /** ×ó¼üÊÍ·Å»Øµ÷ */
        if(str.GetString() == "LBUTTONUP")
        {
            m_szOnLButtonUp = pNode->GetText();
        }
        else
        if(str.GetString() == "WIDTH")
        {
            m_ptSize.m_fX = atoi(pNode->GetText());
        }
        else
        if(str.GetString() == "HEIGHT")
        {
            m_ptSize.m_fY = atoi(pNode->GetText());
        }
        else
        if(str.GetString() == "LEFT")
        {
            m_ptPos.m_fX = atoi(pNode->GetText());
        }
        else
        if(str.GetString() == "TOP")
        {
            m_ptPos.m_fY = atoi(pNode->GetText());
        }
        else
        /** ¾²Ì¬±³¾° */
        if(str.GetString() == "BACKGROUND")
        {
            if(!SetSpriteImage(pNode->GetText()))
            {
                printf("±³¾°ÔØÈë´íÎó¡£\n");
            }
        }
        else
        /** ¶¯Ì¬±³¾° */
        if(str.GetString() == "BACKGROUND_DISABLE")
        {
            if(!SetAnimationImage(pNode->GetText(), OUBUTTON_STATE::OUBS_DISABLE))
            {
                printf("±³¾°ÔØÈë´íÎó¡£\n");
            }
        }
        else
        if(str.GetString() == "BACKGROUND_UP")
        {
            if(!SetAnimationImage(pNode->GetText(), OUBUTTON_STATE::OUBS_UP))
            {
                printf("±³¾°ÔØÈë´íÎó¡£\n");
            }
        }
        else
        if(str.GetString() == "BACKGROUND_HOVER")
        {
            if(!SetAnimationImage(pNode->GetText(), OUBUTTON_STATE::OUBS_HOVER))
            {
                printf("±³¾°ÔØÈë´íÎó¡£\n");
            }
        }
        else
        if(str.GetString() == "BACKGROUND_DOWN")
        {
            if(!SetAnimationImage(pNode->GetText(), OUBUTTON_STATE::OUBS_DOWN))
            {
                printf("±³¾°ÔØÈë´íÎó¡£\n");
            }
        }
    }

    m_bInited = true;
    return true;
}

bool OUGUIButton::SetAnimationImage(string filename, OUBUTTON_STATE state)
{
    /** É¾³ýÔ­×´Ì¬ */
    if(m_Sprite[state].m_bStatic)
    {
        SAFEDEL(m_Sprite[state].m_Img.m_pSprite);
    }
    else
    {
        SAFEDEL(m_Sprite[state].m_Img.m_pAnim);
    }
    
    /** Èô²»´æÔÚ¾²Ì¬±³¾°ÔòÊÍ·ÅTEXTURE */
    if((m_Sprite[0].m_bStatic | m_Sprite[1].m_bStatic | m_Sprite[2].m_bStatic | m_Sprite[3].m_bStatic) == 0)
    {
        if(0 != m_hTex) m_pHGE->Texture_Free(m_hTex);
        m_hTex = 0;
    }

    /** ¿ªÊ¼ÔØÈë */
    string szFilename = filename;
    OUPACKFILEDATA data = OUResourcePool::instance().GetResource(szFilename);
    if(data.m_dwFileSize != 0)
    {
        m_Sprite[state].m_bStatic = false;
        m_Sprite[state].m_Img.m_pAnim = new OUAniFile();
        m_Sprite[state].m_Img.m_pAnim->LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize);

        return true;
    }
    else return false;

    return true;
}

bool OUGUIButton::SetSpriteImage(std::string filename)
{
    /** Çå³ýÔ­×´Ì¬ */
    for(int i = 0; i < OUBS_MAX; i++)
    {
        if(m_Sprite[i].m_bStatic)
        {
            SAFEDEL(m_Sprite[i].m_Img.m_pSprite);
        }
        else
        {
            SAFEDEL(m_Sprite[i].m_Img.m_pAnim);
        }
    }
    if(0 != m_hTex)
    {
        m_pHGE->Texture_Free(m_hTex);
        m_hTex = 0;
    }

    string szFilename = filename;
    OUPACKFILEDATA data = OUResourcePool::instance().GetResource(szFilename);
    if(data.m_dwFileSize != 0)
    {
        m_hTex = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize);
        
        if(m_hTex != 0)
        {
            int nTexWidth = m_pHGE->Texture_GetWidth(m_hTex, true) / OUBS_MAX;
            int nTexHeight = m_pHGE->Texture_GetHeight(m_hTex, true);

            for(int i = 0; i < OUBS_MAX; i++)
            {
                int left = i * nTexWidth;
                m_Sprite[i].m_Img.m_pSprite = new hgeSprite(m_hTex, left, 0, nTexWidth, nTexHeight);
                m_Sprite[i].m_bStatic = true;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

void OUGUIButton::OnMouseEnter()
{
    m_State = OUBS_HOVER;
}

void OUGUIButton::OnMouseLeave()
{
    m_State = OUBS_UP;
}

void OUGUIButton::OnMouseLButtonDown()
{
    m_State = OUBS_DOWN;

    OUGUICallBackObject::RunCallBack(m_szOnLButtonDown, this, NULL);
}

void OUGUIButton::OnMouseLButtonUp()
{
    m_State = OUBS_UP;

    OUGUICallBackObject::RunCallBack(m_szOnLButtonUp, this, NULL);
}
