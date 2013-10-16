#ifndef __OUMAPRESOURCETABLE_H__
#define __OUMAPRESOURCETABLE_H__
#pragma once
#include "global.h"
#include "oumapresourceobject.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

#define OUMAP_RES_PATH_TABLE    "tile\\building\\table"
#define OUMAP_TABLE_PATH        "tile\\table\\"

#define OUMAPTABINNER_DEF_COLOR 0xffffffff

enum OUINNER_IMG_TYPE
{
    OUIIT_TABLE,
    OUIIT_SEAT,
    OUIIT_NUMBER,

    OUIIT_MAX
};

typedef struct OUMapTableInnerImg
{
    OUINNER_IMG_TYPE            m_emType;
    POUGUI_IMG                  m_pImg;
    int                         m_nX;
    int                         m_nY;

    DWORD                       m_dwColor;

    int                         m_nLayer;
} OUTABINNERIMG, *POUTABINNERIMG;

///////////////////////////////////////////////////////////

class OUMapResourceTableItem
{
public:
    OUMapResourceTableItem();
    ~OUMapResourceTableItem();

    void                        Update(float fDelta);
    void                        Render(float x, float y, int id);

    bool                        Load(const char* filename);

    static POUGUI_IMG           GetImage(const char* filename);
    static void                 ReleaseAllImage();

    int                         GetWidth()                      { return m_nWidth; }
    int                         GetHeight()                     { return m_nHeight; }

    void                        SetZ(float z);
    float                       GetZ();

    /** 都是相对的坐标 */
    int                         GetSeatByMouse(float x, float y);
    void                        ResumeSeats();
    POUTABINNERIMG              GetLayer(int idx)               { if(idx >= m_Layers.size()) return NULL; return m_Layers[idx]; }

    const static int            LAYER_NOPOS;

private:
    HGE*                        m_pHGE;

    int                         m_nWidth;
    int                         m_nHeight;

    vector<OUTABINNERIMG*>      m_Layers;
    string                      m_szNumberFormat;

    static map<string, POUGUI_IMG> m_pImgMap;
};

///////////////////////////////////////////////////////////

class OUMapResourceTable : public OUMapResourceObject
{
public:
    OUMapResourceTable(void);
    virtual ~OUMapResourceTable(void);

    virtual bool                Load(DWORD dwID);
    virtual void                Update(float fDeltaTime);
    virtual void                Render(float fX, float fY);
    void                        RenderTable(float fX, float fY, int num);

    virtual void                SetZ(float z);
    virtual float               GetZ();

    static void                 ReleaseAllImage();

    /** 都是相对的坐标 */
    int                         GetSeatByMouse(float x, float y);
    void                        ResumeSeats();
    void                        SetColor(int seatIdx, DWORD color);

protected:
    static OUMapResourceTableItem* GetImage(DWORD dwID);

protected:
    static map<int, OUMapResourceTableItem*> m_pTableImages;

    OUMapResourceTableItem*     m_pImg;
    map<int, DWORD>             m_dwColors;
};

#endif
