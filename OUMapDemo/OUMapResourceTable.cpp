#include "StdAfx.h"
#include "OUMapResourceTable.h"
#include <algorithm>
using namespace std;

map<string, POUGUI_IMG> OUMapResourceTableItem::m_pImgMap;
map<int, OUMapResourceTableItem*> OUMapResourceTable::m_pTableImages;

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

OUMapResourceTableItem::OUMapResourceTableItem() :
    m_pHGE(NULL)
{
    m_pHGE = hgeCreate(HGE_VERSION);
}

OUMapResourceTableItem::~OUMapResourceTableItem()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        delete m_Layers[i];
        m_Layers[i] = 0;
    }
    m_Layers.clear();

    m_pHGE->Release();
    m_pHGE = NULL;
}

void OUMapResourceTableItem::SetZ(float z)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(!m_Layers[i]->m_pImg) continue;
        m_Layers[i]->m_pImg->SetZ(z);
    }
}

float OUMapResourceTableItem::GetZ()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(!m_Layers[i]->m_pImg) continue;
        return m_Layers[i]->m_pImg->GetZ();
    }

    return 1.0f;
}

const int OUMapResourceTableItem::LAYER_NOPOS = -1;

int OUMapResourceTableItem::GetSeatByMouse(float x, float y)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_emType != OUIIT_SEAT) continue;
        if(!m_Layers[i]->m_pImg) continue;

        OUPOINT sz = m_Layers[i]->m_pImg->GetSize();

        hgeRect rect(m_Layers[i]->m_nX, m_Layers[i]->m_nY, m_Layers[i]->m_nX + sz.m_fX, m_Layers[i]->m_nY + sz.m_fY);
        if(rect.TestPoint(x, y))
        {
            /** ÏñËØÅö×² */
            if(!m_Layers[i]->m_pImg->m_bStatic) return i;
            else
            {
                HTEXTURE hTex = m_Layers[i]->m_pImg->GetSprite()->GetTexture();
                int nx, ny, nw, nh;
                nx = x - m_Layers[i]->m_nX, ny = y - m_Layers[i]->m_nY, nw = m_pHGE->Texture_GetWidth(hTex), nh = m_pHGE->Texture_GetHeight(hTex);

                DWORD* mem = m_pHGE->Texture_Lock(hTex);
                DWORD color = mem[ny * nw + nx];
                m_pHGE->Texture_Unlock(hTex);

                if(color) return i;
            }
        }
    }

    return LAYER_NOPOS;
}

void OUMapResourceTableItem::ResumeSeats()
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_emType != OUIIT_SEAT) continue;
        if(!m_Layers[i]->m_pImg) continue;

        m_Layers[i]->m_dwColor = OUMAPTABINNER_DEF_COLOR;
    }
}

bool OUMapResourceTableItem::Load(const char *filename)
{
    OUIniFile Ini(filename);

    m_nWidth = Ini.GetInt("main", "width");
    m_nHeight = Ini.GetInt("main", "height");

    int nSeats = Ini.GetInt("main", "seat");
    char sec[20];
    for(int i = 0; i < nSeats; i++)
    {
        sprintf(sec, "seat%d", i + 1);
        POUTABINNERIMG pImg = new OUTABINNERIMG();
        pImg->m_emType = OUIIT_SEAT;
        pImg->m_pImg = OUMapResourceTableItem::GetImage(Ini.GetString(sec, "img", "null.png").c_str());
        pImg->m_nX = Ini.GetInt(sec, "x");
        pImg->m_nY = Ini.GetInt(sec, "y");
        pImg->m_nLayer = Ini.GetInt(sec, "lay");
        pImg->m_dwColor = OUMAPTABINNER_DEF_COLOR;

        m_Layers.push_back(pImg);
    }

    sprintf(sec, "table");
    POUTABINNERIMG pImg = new OUTABINNERIMG();
    pImg->m_emType = OUIIT_TABLE;
    pImg->m_pImg = OUMapResourceTableItem::GetImage(Ini.GetString(sec, "img", "null.png").c_str());
    pImg->m_nX = Ini.GetInt(sec, "x");
    pImg->m_nY = Ini.GetInt(sec, "y");
    pImg->m_nLayer = Ini.GetInt(sec, "lay");
    pImg->m_dwColor = OUMAPTABINNER_DEF_COLOR;
    m_Layers.push_back(pImg);

    /** Êý×Ö */
    sprintf(sec, "num");
    pImg = new OUMapTableInnerImg();
    pImg->m_emType = OUIIT_NUMBER;
    pImg->m_pImg = NULL;
    pImg->m_nX = Ini.GetInt(sec, "x");
    pImg->m_nY = Ini.GetInt(sec, "y");
    pImg->m_dwColor = OUMAPTABINNER_DEF_COLOR;
    pImg->m_nLayer = Ini.GetInt(sec, "lay");
    m_Layers.push_back(pImg);
    m_szNumberFormat = Ini.GetString(sec, "img", "%d.png");

    sort(m_Layers.begin(), m_Layers.end(), __innerimg_cmp);

    return true;
}

void OUMapResourceTableItem::Update(float fDelta)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->Update(fDelta);
        }
    }
}

void OUMapResourceTableItem::Render(float x, float y, int id)
{
    for(int i = 0; i < m_Layers.size(); i++)
    {
        if(m_Layers[i]->m_pImg)
        {
            m_Layers[i]->m_pImg->SetColor(m_Layers[i]->m_dwColor);
            m_Layers[i]->m_pImg->Render(x + m_Layers[i]->m_nX, y + m_Layers[i]->m_nY);
            m_Layers[i]->m_pImg->SetColor(OUMAPTABINNER_DEF_COLOR);
        }
        else
        if(m_Layers[i]->m_emType == OUIIT_NUMBER)
        {
            POUGUI_IMG number;
            static char num[512];
            sprintf(num, m_szNumberFormat.c_str(), id);
            number = OUMapResourceTableItem::GetImage(num);
            if(number)
            {
                number->SetColor(m_Layers[i]->m_dwColor);
                number->Render(x + m_Layers[i]->m_nX, y + m_Layers[i]->m_nY);
                number->SetColor(OUMAPTABINNER_DEF_COLOR);
            }
        }
    }
}

POUGUI_IMG OUMapResourceTableItem::GetImage(const char *filename)
{
    if(!g_pHGE) return NULL;

    OUStringFunc str(filename);
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
    
    /** ¶¯»­ */
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
        
        HTEXTURE hTex = g_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize, true);
        if(0 == hTex) return NULL;

        pImg->m_Img.m_pSprite = new hgeSprite(hTex, 0, 0, g_pHGE->Texture_GetWidth(hTex, true), g_pHGE->Texture_GetHeight(hTex, true));

        m_pImgMap[str.GetString()] = pImg;
        return pImg;
    }

    return NULL;
}

void OUMapResourceTableItem::ReleaseAllImage()
{
    for(map<string, POUGUI_IMG>::iterator it = m_pImgMap.begin(); it != m_pImgMap.end(); it++)
    {
        if(it->second->m_bStatic)
        {
            HTEXTURE hTex = it->second->m_Img.m_pSprite->GetTexture();
            it->second->Release();
            g_pHGE->Texture_Free(hTex);
        }
        else it->second->Release();

        SAFEDEL(it->second);
    }
    m_pImgMap.clear();
}
///////////////////////////////////////////////////

OUMapResourceTable::OUMapResourceTable(void)
{
}

OUMapResourceTable::~OUMapResourceTable(void)
{
}

bool OUMapResourceTable::Load(DWORD dwID)
{
    m_pImg = NULL;
    m_pImg = OUMapResourceTable::GetImage(dwID);
    if(!m_pImg) return false;

    m_fWidth = m_pImg->GetWidth();
    m_fHeight = m_pImg->GetHeight();

    m_dwID = dwID;

    return true;
}

void OUMapResourceTable::Update(float fDeltaTime)
{
    if(!m_pImg) return;
    m_pImg->Update(fDeltaTime);
}

void OUMapResourceTable::Render(float fX, float fY)
{
    if(!m_pImg) return;

    POUTABINNERIMG img;
    for(map<int, DWORD>::iterator it = m_dwColors.begin(); it != m_dwColors.end(); it++)
    {
        img = m_pImg->GetLayer(it->first);
        if(NULL != img) img->m_dwColor = it->second;
    }

    m_pImg->Render(fX, fY, -1);

    m_pImg->ResumeSeats();
}

void OUMapResourceTable::RenderTable(float fX, float fY, int num)
{
    if(!m_pImg) return;

    POUTABINNERIMG img;
    for(map<int, DWORD>::iterator it = m_dwColors.begin(); it != m_dwColors.end(); it++)
    {
        img = m_pImg->GetLayer(it->first);
        if(NULL != img) img->m_dwColor = it->second;
    }

    m_pImg->Render(fX, fY, num);

    m_pImg->ResumeSeats();
}

void OUMapResourceTable::SetZ(float z)
{
    if(!m_pImg) return;
    m_pImg->SetZ(z);
}

float OUMapResourceTable::GetZ()
{
    if(!m_pImg) return 1.0f;
    return m_pImg->GetZ();
}

int OUMapResourceTable::GetSeatByMouse(float x, float y)
{
    if(!m_pImg) return OUMapResourceTableItem::LAYER_NOPOS;
    return m_pImg->GetSeatByMouse(x, y);
}

void OUMapResourceTable::ResumeSeats()
{
    if(!m_pImg) return;

    m_dwColors.clear();
}

void OUMapResourceTable::SetColor(int seatIdx, DWORD color)
{
    m_dwColors[seatIdx] = color;
}

OUMapResourceTableItem* OUMapResourceTable::GetImage(DWORD dwID)
{
    if(m_pTableImages.find(dwID) != m_pTableImages.end())
    {
        return m_pTableImages[dwID];
    }

    static char filename[512];
    sprintf(filename, "%s%d%s", OUMAP_RES_PATH_TABLE, dwID, ".ini");

    OUMapResourceTableItem* item = new OUMapResourceTableItem();
    if(!item->Load(filename))
    {
        delete item;
        return NULL;
    }

    m_pTableImages[dwID] = item;
    return m_pTableImages[dwID];
}

void OUMapResourceTable::ReleaseAllImage()
{
    for(map<int, OUMapResourceTableItem*>::iterator it = m_pTableImages.begin(); it != m_pTableImages.end(); it++)
    {
        delete it->second;
    }
    m_pTableImages.clear();

    OUMapResourceTableItem::ReleaseAllImage();
}

