#include "OUAniFile.h"

map<string, ouaniinfo> OUAniFile::__OUANITEXTURE;
HGE* OUAniFile::m_pHGE = NULL;

OUAniFile::OUAniFile(void) :
    m_hTex(0),
    m_pAnim(NULL)
{
}

OUAniFile::~OUAniFile(void)
{
    if(!m_pAnim) delete m_pAnim;
    m_pHGE->Release();
}

bool OUAniFile::LoadFromMemory(char* mem, int size)
{
    SAFEDEL(m_pAnim);
    int nHeaderSize = sizeof(ouaheader);
    memcpy(&m_Header, mem, nHeaderSize);
    
    if(m_Header.magic_num != OUAF_MAGIC_NUM) return false;

    DWORD dwSize = sizeof(DWORD);
    DWORD tmpclr;
    DWORD cur = nHeaderSize;
    for(int i = 0; i < m_Header.clr_num; i++)
    {
        memcpy(&tmpclr, mem + cur, dwSize);
        m_dwDict.push_back(tmpclr);
        cur += dwSize;
    }

    m_hTex = m_pHGE->Texture_Create(m_Header.width, m_Header.height * m_Header.pic_num);
    if(0 == m_hTex) return false;

    pixinfo pinfo;
    int repeated;
    DWORD usSize = sizeof(unsigned short);
    DWORD uiSize = sizeof(unsigned int);

    DWORD* pStart = m_pHGE->Texture_Lock(m_hTex);
    DWORD offset = m_Header.width * m_Header.height;
    for(int i = 0; i < m_Header.pic_num; i++)
    {
        DWORD* pInStart = pStart + offset * i;
        repeated = 0;
        pinfo.repeat = 0;
        DWORD nowsize = 0;

        for(; nowsize < offset;)
        {
            pinfo.idx = 0;
            memcpy((char*)(&pinfo.idx), mem + cur, m_Header.clr_byte);
            cur += usSize;
            memcpy(&pinfo.repeat, mem + cur, usSize);
            cur += usSize;

            for(int j = 0; j < pinfo.repeat; j++)
            {
                pInStart[nowsize + j] = m_dwDict[pinfo.idx];
            }
            nowsize += pinfo.repeat;
        }
    }
    m_pHGE->Texture_Unlock(m_hTex);

    m_pAnim = new hgeAnimation(m_hTex, m_Header.pic_num, m_Header.fps, 0, 0, m_Header.width, m_Header.height);
    m_pAnim->SetHotSpot(m_Header.spotx, m_Header.spoty);

    return true;
}

bool OUAniFile::LoadFromTexture(HTEXTURE hTex, int nWidth, int nHeight, int count, int fps, int hotspotx, int hotspoty)
{
    SAFEDEL(m_pAnim);
    if(!hTex) return false;

    m_hTex = hTex;
    m_pAnim = new hgeAnimation(m_hTex, count, fps, 0, 0, nWidth, nHeight);
    m_pAnim->SetHotSpot(hotspotx, hotspoty);

    return true;
}
