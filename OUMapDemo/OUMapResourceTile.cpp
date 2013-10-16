#include "StdAfx.h"
#include "OUMapResourceTile.h"

OUMapResourceTile::OUMapResourceTile(void) :
    m_pImg(NULL),
    m_hTex(0)
{
}

OUMapResourceTile::~OUMapResourceTile(void)
{
    Release();
}

void OUMapResourceTile::Release()
{
    if(0 != m_hTex)
    {
        m_pHGE->Texture_Free(m_hTex);
    }
    SAFEDEL(m_pImg);

    m_fWidth = m_fHeight = 0.0f;
}

bool OUMapResourceTile::Load(DWORD dwID)
{
    Release();

    string filename;
    for(int i = 0; i < OUMAP_RES_MAX_EXT; i++)
    {
        filename = MakeFilePath(dwID, g_szMapResExt[i]);

        OUPACKFILEDATA data = OUResourcePool::instance().GetResource(filename);
        if(data.m_dwFileSize == 0) continue;

        if(!strcmp(g_szMapResExt[i], ".oga"))
        {
            m_pImg = new OUGUI_IMG();
            m_pImg->m_bStatic = false;
            m_pImg->m_Img.m_pAnim = new OUAniFile();
            if(!m_pImg->m_Img.m_pAnim->LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize))
            {
                SAFEDEL(m_pImg);
                continue;
            }

            m_fWidth = m_pImg->GetAnimation()->GetWidth();
            m_fHeight = m_pImg->GetAnimation()->GetHeight();
            m_pImg->Play();

            return true;
        }
        else
        {
            m_hTex = m_pHGE->Texture_Load((const char*)data.m_pBuf, data.m_dwFileSize, true);
            if(!m_hTex)
            {
                continue;
            }

            m_fWidth = m_pHGE->Texture_GetWidth(m_hTex, true);
            m_fHeight = m_pHGE->Texture_GetHeight(m_hTex, true);

            /** Í¸Ã÷É« */
            float fDummyWidth = m_pHGE->Texture_GetWidth(m_hTex);
            float fDummyHeight = m_pHGE->Texture_GetHeight(m_hTex);
            int imax = (int)(fDummyWidth) * (int)(fDummyHeight);
            DWORD* clr = m_pHGE->Texture_Lock(m_hTex, false, 0, 0, fDummyWidth, fDummyHeight);
            for(int i = 0; i < imax; i++)
            {
                if(clr[i] == OUMRI_TRANSPARENT_COLOR)
                {
                    clr[i] = 0x00000000;
                }
            }
            clr = 0;
            m_pHGE->Texture_Unlock(m_hTex);

            m_pImg = new OUGUI_IMG();
            m_pImg->m_bStatic = true;
            m_pImg->m_Img.m_pSprite = new hgeSprite(m_hTex, 0, 0, m_fWidth, m_fHeight);

            return true;
        }
    }

    return false;
}

void OUMapResourceTile::Update(float fDeltaTime)
{
    if(!m_pImg) return;
    m_pImg->Update(fDeltaTime);
}

void OUMapResourceTile::Render(float fX, float fY)
{
    if(!m_pImg) return;
    m_pImg->Render(fX, fY);
}

void OUMapResourceTile::RenderStretch(float x1, float y1, float w, float h)
{
    if(!m_pImg) return;
    m_pImg->RenderStretch(x1, y1, w, h);
}

void OUMapResourceTile::SetColor(DWORD dwColor)
{
    if(!m_pImg) return;
    m_pImg->SetColor(dwColor);
}

DWORD OUMapResourceTile::GetColor()
{
    if(!m_pImg) return 0;
    return m_pImg->GetColor();
}

void OUMapResourceTile::SetZ(float z)
{
    if(!m_pImg) return;
    m_pImg->SetZ(z);
}

float OUMapResourceTile::GetZ()
{
    if(!m_pImg) return 1.0f;
    return m_pImg->GetZ();
}

const char* OUMapResourceTile::MakeFilePath(DWORD dwID, const char* szExt)
{
    static char filename[256];

    sprintf(filename, "%s%d%s", OUMAP_RES_PATH_TILE, dwID, szExt);
    return filename;
}
