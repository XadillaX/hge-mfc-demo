#ifndef __DEMOGLOBAL_H__
#define __DEMOGLOBAL_H__

#include "../OUPlazaRender/hge.h"
#include "../OUPlazaRender/hgeRect.h"
#include "../OUPlazaRender/OUStringFunc.h"
#include "../OUPlazaRender/OUIniFile.h"
#include "../OUPlazaGUISystem/Global.h"
#include "../OUPlazaGUISystem/OUFont.h"

class OURender;

#ifndef SAFEDEL
#define SAFEDEL(p)                          { if(p != NULL) { delete p; p = NULL; } }
#endif

#define OUSCREEN_WIDTH                      800
#define OUSCREEN_HEIGHT                     600

#define OUMRI_TRANSPARENT_COLOR             0xffff00ff
#define OUIDE_HGE_TIMER                     555
#define OUOGM_MGC_NUMBER                    0xC2D4C0CB

#define OUMAP_MAX_WIDTH                     1000
#define OUMAP_MAX_HEIGHT                    1000

extern bool OUHGE_UPDATE();
extern bool OUHGE_RENDER();
extern bool OUHGE_RESTORETARGET();
extern HGE* g_pHGE;
extern OURender* g_pRender;

typedef struct tagOUMAPFILEHEADER
{
    DWORD                       m_dwMagicNum;
    char                        m_szMapName[64];

    DWORD                       m_dwTileWidth;
    DWORD                       m_dwTileHeight;

    DWORD                       m_dwMapWidth;
    DWORD                       m_dwMapHeight;

    /** 物体数 */
    DWORD                       m_dwObjectCount;

    /** 头大小 */
    DWORD                       m_dwHeaderSize;
} OUMAPFILEHEADER, *POUMAPFILEHEADER;

enum OUMAP_RES_TYPE
{
    OUMRT_TILE,
    OUMRT_CARPET,
    OUMRT_BUILDING,
    OUMRT_TABLE,

    OUMRT_MAX
};

struct OUMapCoor
{
    int                         x;
    int                         y;
};

extern bool operator < (const OUMapCoor a, const OUMapCoor b);

#endif
