#include "StdAfx.h"
#include "OUGUIPanel.h"

REGISTERGUI("panel", OUGUIPanel, __ou_gui_panel_creator);

OUGUIPanel::OUGUIPanel(void) :
    m_hTex(0),
    m_dwHitColor(0xffffffff)
{
}

OUGUIPanel::~OUGUIPanel(void)
{
    m_Sprite.Release();
    if(0 != m_hTex) m_pHGE->Texture_Free(m_hTex);
}

void OUGUIPanel::OnRender()
{
    OUPOINT ap = GetAbsolutePos();
    //if(m_Sprite)
    //{
        //m_pSprite->SetColor(m_dwHitColor);
        m_Sprite.Render(ap.m_fX, ap.m_fY);
    //}

    //OnDrawBoard(0xff00ff00);
    //OUFontLoader::instance().GetFont(string("宋体"))->Print(ap.m_fX + 5, ap.m_fY + 5, "鼠标位置: (%d, %d)", (int)m_ptMousePos.m_fX, (int)m_ptMousePos.m_fY);
}

bool OUGUIPanel::OnLoadFromXml(TiXmlElement *pElement, OUGUIObject *pWillParent)
{
    /** 释放原先的对象 */
    m_Sprite.Release();
    if(m_hTex != 0) m_pHGE->Texture_Free(m_hTex);

    OUGUIObject::OnLoadFromXml(pElement, pWillParent);

    TiXmlElement* pNode;
    for(pNode = pElement->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
    {
        Assert(pNode->Value(), "Empty node value.");
        OUStringFunc str = pNode->Value();
        str.ToUppercase();

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
        if(str.GetString() == "BACKGROUND")
        {
            string szFilename = pNode->GetText();
            OUPACKFILEDATA data = OUResourcePool::instance().GetResource(szFilename);

            if(data.m_dwFileSize != 0)
            {
                /** 判断后缀 : 是否动画文件 */
                OUStringFunc tmp(szFilename);
                tmp.ToUppercase();
                if(tmp.GetExt() == ".OGA")
                {
                    m_Sprite.m_bStatic = false;
                    m_Sprite.m_Img.m_pAnim = new OUAniFile();
                    m_Sprite.m_Img.m_pAnim->LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize);
                    m_Sprite.m_Img.m_pAnim->GetAnimation()->Play();

                    Assert(m_Sprite.m_Img.m_pAnim, "精灵动画创建失败。");
                }
                else
                {
                    m_hTex = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize);
                    
                    if(m_hTex != 0)
                    {
                        int nTexWidth = m_pHGE->Texture_GetWidth(m_hTex);
                        int nTexHeight = m_pHGE->Texture_GetHeight(m_hTex);
                        m_Sprite.m_Img.m_pSprite = new hgeSprite(m_hTex, 0, 0, nTexWidth, nTexHeight);
                        m_Sprite.m_bStatic = true;

                        Assert(m_Sprite.m_Img.m_pSprite, "精灵创建失败。\n");
                    }
                }
            }
            else
            {
                printf("背景载入错误。\n");
            }
        }
    }

    m_bInited = true;
    return true;
}

bool OUGUIPanel::OnCheckCollision(float fMX, float fMY)
{
    if(NULL == m_Sprite.GetSprite())
    {
        if(NULL == m_Sprite.GetAnimation()) return OUGUIObject::OnCheckCollision(fMX, fMY);

        /** 获取精灵以及纹理宽高 */
        int sw = m_Sprite.GetAnimation()->GetWidth();
        int sh = m_Sprite.GetAnimation()->GetHeight();
        int hw = m_pHGE->Texture_GetWidth(m_Sprite.GetAnimation()->GetTexture());
        int hh = m_pHGE->Texture_GetHeight(m_Sprite.GetAnimation()->GetTexture()) / m_Sprite.GetAnimation()->GetFrames();
        
        /** 在精灵外面了 */
        if(!((fMX >= m_ptPos.m_fX) && (fMX <= m_ptPos.m_fX + sw) && (fMY >= m_ptPos.m_fY) && (fMY <= m_ptPos.m_fY + sh)))
        {
            return false;
        }

        float px, py;
        m_Sprite.GetAnimation()->GetCurFramePosInTexture(&px, &py);

        /** 锁住纹理——获取纹理缓冲区 */
        DWORD* mem = m_pHGE->Texture_Lock(m_Sprite.GetAnimation()->GetTexture(), true, px, py, hw, hh);
        if(mem == 0)
        {
            m_pHGE->Texture_Unlock(m_Sprite.GetAnimation()->GetTexture());
            return OUGUIObject::OnCheckCollision(fMX, fMY);
        }

        int x, y;
        x = (int)(fMX - m_ptPos.m_fX) % hw;
        y = (int)(fMY - m_ptPos.m_fY) % hh;
        bool res = (mem[y * hw + x] == 0) ? false : true;
        m_pHGE->Texture_Unlock(m_Sprite.GetAnimation()->GetTexture());

        return res;
    }
    else
    {
        /** 获取精灵以及纹理宽高 */
        int sw = m_Sprite.GetSprite()->GetWidth();
        int sh = m_Sprite.GetSprite()->GetHeight();
        int hw = m_pHGE->Texture_GetWidth(m_Sprite.GetSprite()->GetTexture());
        int hh = m_pHGE->Texture_GetHeight(m_Sprite.GetSprite()->GetTexture());

        /** 在精灵外面了 */
        if(!((fMX >= m_ptPos.m_fX) && (fMX <= m_ptPos.m_fX + sw) && (fMY >= m_ptPos.m_fY) && (fMY <= m_ptPos.m_fY + sh)))
        {
            return false;
        }

        /** 锁住纹理——获取纹理缓冲区 */
        DWORD* mem = m_pHGE->Texture_Lock(m_Sprite.GetSprite()->GetTexture(), true);
        if(mem == 0)
        {
            m_pHGE->Texture_Unlock(m_Sprite.GetSprite()->GetTexture());
            return OUGUIObject::OnCheckCollision(fMX, fMY);
        }

        int x, y;
        x = (int)(fMX - m_ptPos.m_fX) % hw;
        y = (int)(fMY - m_ptPos.m_fY) % hh;
        bool res = (mem[y * hw + x] == 0) ? false : true;
        m_pHGE->Texture_Unlock(m_Sprite.GetSprite()->GetTexture());

        return res;
    }
}
