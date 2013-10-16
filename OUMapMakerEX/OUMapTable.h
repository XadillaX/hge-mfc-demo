#ifndef __OUMAPTABLE_H__
#define __OUMAPTABLE_H__
#pragma once
#include "oumapview.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

#define OUMAP_TABLE_PATH    "Tile/Table/"

enum OUINNER_IMG_TYPE
{
    OUIIT_TABLE,
    OUIIT_SEAT,
    OUIIT_NUMBER,

    OUIIT_MAX
};

typedef struct OUMapTableInnerImg
{
    OUINNER_IMG_TYPE        m_emType;
    POUGUI_IMG              m_pImg;
    int                     m_nX;
    int                     m_nY;

    int                     m_nLayer;
} OUTABINNERIMG, *POUTABINNERIMG;

class OUMapTableImg : public OUGUI_IMG
{
public:
    OUMapTableImg();
    ~OUMapTableImg();

    bool                    LoadFromIni(const char* path);

    static void             InitAll();
    static void             ReleaseAll();
    static void             Clear();
    static POUGUI_IMG       GetImage(const char* path);

    virtual OUPOINT         GetSize()
    {
        OUPOINT pt = { m_nWidth, m_nHeight };
        return pt;
    }

    virtual void            Render(float x, float y);
    void                    RenderWith(float x, float y, int id);
    virtual void            SetColor(DWORD color);
    virtual void            RenderStretch(float x, float y, float w, float h);
    virtual void            Update(float fDelta);
    virtual void            Play();
    virtual void            Stop();
    virtual void            Resume();
    virtual bool            IsPlaying();
    virtual void            SetZ(float z);
    virtual void            SetSpot(float x, float y);
    virtual bool            GetSpot(float* x, float* y);

    bool                    TestPoint(int x, int y);

    virtual hgeSprite*      GetSprite()                 { return NULL; }
    virtual hgeAnimation*   GetAnimation()              { return NULL; }

private:
    static map<string, POUGUI_IMG> m_pImgMap;
    static HGE*             m_pHGE;

    int                     m_nWidth;
    int                     m_nHeight;

    vector<OUTABINNERIMG*>  m_Layers;

    string                  m_szNumberFormat;
};

class OUMapTable : public tagOUMAP_UPPER_CELL
{
public:
    OUMapTable(void);
    ~OUMapTable(void);

    static void             RenewID();
    //void                    SetID();

    int                     GetTableID()
    {
        return m_nID;
    }

private:
    static vector<OUMapTable*> m_Array;
    static int              m_nNextID;
    static bool             m_bIDUsed[65535];

    int                     m_nID;
};

#endif
