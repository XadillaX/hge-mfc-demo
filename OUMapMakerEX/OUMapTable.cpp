#include "StdAfx.h"
#include "OUMapTable.h"
#include "../OUPlazaRender/OUStringFunc.h"
#include "../OUPlazaRender/OUResourceManager.h"

map<string, POUGUI_IMG> OUMapTableImg::m_pImgMap;
HGE* OUMapTableImg::m_pHGE;

OUMapTableImg::OUMapTableImg(void) :
    m_nWidth(0),
    m_nHeight(0)
{
    m_Layers.clear();
    m_bStatic = true;
    m_Img.m_pSprite = NULL;
}

OUMapTableImg::~OUMapTableImg(void)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        delete m_Layers[i];
    }
}

#include "../OUPlazaRender/OUIniFile.h"
#include <algorithm>

bool __innerimg_cmp(POUTABINNERIMG a, POUTABINNERIMG b)
{
    if(a->m_nLayer < b->m_nLayer) return true;
    else
    if(a->m_nLayer > b->m_nLayer) return false;
    else
    if(a->m_nX < b->m_nX) return true;
    else
    if(a->m_nX > b->m_nX) return false;
    else
    if(a->m_nY < b->m_nY) return true;
    else
    if(a->m_nY > b->m_nY) return false;
    else
    return a->m_emType < b->m_emType;
}

bool OUMapTableImg::LoadFromIni(const char *path)
{
    OUIniFile Ini(path);
    m_nWidth = Ini.GetInt("main", "width");
    m_nHeight = Ini.GetInt("main", "height");

    /** 椅子 */
    int nSeats = Ini.GetInt("main", "seat");
    char sec[20];
    for(int i = 0; i < nSeats; i++)
    {
        sprintf(sec, "seat%d", i + 1);
        OUMapTableInnerImg* pImg = new OUMapTableInnerImg();
        pImg->m_emType = OUIIT_SEAT;
        pImg->m_pImg = OUMapTableImg::GetImage(Ini.GetString(sec, "img", "null.png").c_str());
        pImg->m_nX = Ini.GetInt(sec, "x");
        pImg->m_nY = Ini.GetInt(sec, "y");
        pImg->m_nLayer = Ini.GetInt(sec, "lay");

        m_Layers.push_back(pImg);
    }

    /** 桌子 */
    sprintf(sec, "table");
    OUMapTableInnerImg* pImg = new OUMapTableInnerImg();
    pImg->m_emType = OUIIT_TABLE;
    pImg->m_pImg = OUMapTableImg::GetImage(Ini.GetString(sec, "img", "null.png").c_str());
    pImg->m_nX = Ini.GetInt(sec, "x");
    pImg->m_nY = Ini.GetInt(sec, "y");
    pImg->m_nLayer = Ini.GetInt(sec, "lay");
    m_Layers.push_back(pImg);

    /** 数字 */
    sprintf(sec, "num");
    pImg = new OUMapTableInnerImg();
    pImg->m_emType = OUIIT_NUMBER;
    pImg->m_pImg = NULL;
    pImg->m_nX = Ini.GetInt(sec, "x");
    pImg->m_nY = Ini.GetInt(sec, "y");
    pImg->m_nLayer = Ini.GetInt(sec, "lay");
    m_Layers.push_back(pImg);
    m_szNumberFormat = Ini.GetString(sec, "img", "%d.png");

    sort(m_Layers.begin(), m_Layers.end(), __innerimg_cmp);

    /** 动静态 */
    m_bStatic = true;
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg && !m_Layers[i]->m_pImg->m_bStatic)
        {
            m_bStatic = false;
        }
    }

    return true;
}

void OUMapTableImg::Render(float x, float y)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Render(x + m_Layers[i]->m_nX, y + m_Layers[i]->m_nY);
        }
    }
}

void OUMapTableImg::RenderWith(float x, float y, int id)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Render(x + m_Layers[i]->m_nX, y + m_Layers[i]->m_nY);
        }
        else
        if(m_Layers[i]->m_emType == OUIIT_NUMBER)
        {
            POUGUI_IMG number;
            static char num[512];
            sprintf(num, m_szNumberFormat.c_str(), id);
            number = GetImage(num);
            if(number)
            {
                number->Render(x + m_Layers[i]->m_nX, y + m_Layers[i]->m_nY);
            }
        }
    }
}

bool OUMapTableImg::TestPoint(int x, int y)
{
    static hgeRect rect;
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg && m_Layers[i]->m_pImg->m_bStatic)
        {
            HTEXTURE hTex = m_Layers[i]->m_pImg->m_Img.m_pSprite->GetTexture();
            int w, h;
            w = m_pHGE->Texture_GetWidth(hTex);
            h = m_pHGE->Texture_GetHeight(hTex);

            rect.Set(m_Layers[i]->m_nX, m_Layers[i]->m_nY, m_Layers[i]->m_nX + w, m_Layers[i]->m_nY + h);
            if(!rect.TestPoint(x, y)) continue;

            DWORD* pBuff = m_pHGE->Texture_Lock(hTex);
            DWORD ok = pBuff[(y - m_Layers[i]->m_nY) * w + (x - m_Layers[i]->m_nX)];
            m_pHGE->Texture_Unlock(hTex);

            if(ok) return true;
        }
    }

    return false;
}

void OUMapTableImg::SetColor(DWORD color)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->SetColor(color);
        }
    }
}

void OUMapTableImg::RenderStretch(float x, float y, float w, float h)
{
    /** TODO */
}

void OUMapTableImg::Update(float fDelta)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Update(fDelta);
        }
    }
}

void OUMapTableImg::Play()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Play();
        }
    }
}

void OUMapTableImg::Stop()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Stop();
        }
    }
}

void OUMapTableImg::Resume()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Resume();
        }
    }
}

bool OUMapTableImg::IsPlaying()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg && !m_Layers[i]->m_pImg->m_bStatic)
        {
            return m_Layers[i]->m_pImg->IsPlaying();
        }
    }

    return false;
}

void OUMapTableImg::SetZ(float z)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->SetZ(z);
        }
    }
}

void OUMapTableImg::SetSpot(float x, float y)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->SetSpot(x, y);
        }
    }
}

bool OUMapTableImg::GetSpot(float* x, float* y)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->GetSpot(x, y);
            return true;
        }
    }

    return false;
}

void OUMapTableImg::InitAll()
{
    m_pHGE = hgeCreate(HGE_VERSION);
}

void OUMapTableImg::ReleaseAll()
{
    if(!m_pHGE) return;

    Clear();
    m_pHGE->Release();
    m_pHGE = NULL;
}

void OUMapTableImg::Clear()
{
    for(map<string, POUGUI_IMG>::iterator it = m_pImgMap.begin(); it != m_pImgMap.end(); it++)
    {
        if(it->second->m_bStatic)
        {
            HTEXTURE hTex = it->second->m_Img.m_pSprite->GetTexture();
            it->second->Release();
            m_pHGE->Texture_Free(hTex);
        }
        else it->second->Release();

        SAFEDEL(it->second);
    }
    m_pImgMap.clear();
}

POUGUI_IMG OUMapTableImg::GetImage(const char *path)
{
    if(!m_pHGE) return NULL;

    OUStringFunc str(path);
    OUStringFunc str2;
    str.FormatPath(OUStringFunc::OUPT_UPPERCASE);

    if(m_pImgMap.find(str.GetString()) != m_pImgMap.end())
    {
        return m_pImgMap[str.GetString()];
    }

    str2.SetString(OUMAP_TABLE_PATH + str.GetString());
    str2.FormatPath(OUStringFunc::OUPT_UPPERCASE);

    OUPACKFILEDATA data = OUResourcePool::instance().GetResource(str2.GetString());
    if(data.m_dwFileSize == 0) return NULL;
    
    POUGUI_IMG pImg = new OUGUI_IMG();
    
    /** 动画 */
    if(str.GetExt() == ".OGA")
    {
        pImg->m_bStatic = false;
        pImg->m_Img.m_pAnim = new OUAniFile();
        if(!pImg->m_Img.m_pAnim->LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize))
        {
            delete pImg;
            return NULL;
        }
        pImg->m_Img.m_pAnim->GetAnimation()->Play();

        m_pImgMap[str.GetString()] = pImg;
        return pImg;
    }
    else
    {
        pImg->m_bStatic = true;
        
        HTEXTURE hTex = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize, true);
        if(0 == hTex) return NULL;

        pImg->m_Img.m_pSprite = new hgeSprite(hTex, 0, 0, m_pHGE->Texture_GetWidth(hTex, true), m_pHGE->Texture_GetHeight(hTex, true));

        m_pImgMap[str.GetString()] = pImg;
        return pImg;
    }

    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////

int OUMapTable::m_nNextID = 1;
bool OUMapTable::m_bIDUsed[65535] = { 0 };
vector<OUMapTable*> OUMapTable::m_Array;

OUMapTable::OUMapTable()
{
    m_emObjectType = OMOT_TABLE;
    while(m_bIDUsed[m_nNextID] && m_nNextID < 65536) m_nNextID++;
    m_nID = m_nNextID++;
    m_bIDUsed[m_nID] = true;

    m_Array.push_back(this);

    RenewID();
}

OUMapTable::~OUMapTable()
{
    m_bIDUsed[m_nID] = false;
    for(vector<OUMapTable*>::iterator it = m_Array.begin(); it != m_Array.end(); it++)
    {
        if(*it == this)
        {
            m_Array.erase(it);
            break;
        }
    }

    RenewID();
}

void OUMapTable::RenewID()
{
    for(int i = 0; i < m_Array.size(); i++)
    {
        m_Array[i]->m_nID = i + 1;
    }
    m_nNextID = m_Array.size() + 1;
    memset(m_bIDUsed, 0, sizeof(m_bIDUsed));
    for(int i = 1; i <= m_Array.size(); i++) m_bIDUsed[i] = true;
}
