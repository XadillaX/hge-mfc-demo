/***********************************
 *
 * O游棋牌大厅世界
 * - 地图测试 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUMAPTESTER_H__
#define __OUMAPTESTER_H__
#pragma once
#include "../OUPlazaRender/hge.h"
#include "../OUPlazaGUISystem/Global.h"
#include <map>
#include <set>
#include <vector>
#include "../OUPlazaGUISystem/OUFont.h"
using namespace std;

#define OUMAP_TILE_NAME_FORMAT      "Assets/tile/road/tile%d"
#define OUMAP_MAP_NAME_FORMAT       "Assets/map/%s.map"
#define OUMAP_BUILDING_NAME_FORMAT  "Assets/tile/building/wall%d"
#define OUMAP_BMAP_NAME_FORMAT      "Assets/map/%sbuilding.map"

#define OUMAP_TILE_WIDTH            64
#define OUMAP_TILE_HEIGHT           32
#define HALF_TILE_WIDTH             (OUMAP_TILE_WIDTH / 2)
#define HALF_TILE_HEIGHT            (OUMAP_TILE_HEIGHT / 2)

/** 纹理文件引用次数（以地图为单位） */
typedef struct tagOUMAP_TEX_REF
{
    DWORD                       id;
    HTEXTURE                    tex;
    UINT                        time;
    
    OUGUI_IMG                   img;

    tagOUMAP_TEX_REF()
    {
        id = 0, tex = 0, time = 0;
    }
} OUMAP_TEX_REF, *POUMAP_TEX_REF;

typedef struct tagOUMAP_CELL
{
    DWORD                       m_BottomID;
    DWORD                       m_BuildingID;
    //DWORD                       m_SkyID;

    POUGUI_IMG                  m_pBottomImg;
    POUGUI_IMG                  m_pBuildingImg;

    bool                        block;

    int                         x;
    int                         y;
} OUMAP_CELL, *POUMAP_CELL;

typedef struct tagOUMAP_BUILDING
{
    int x;
    int y;
}OUMAP_BUILDING, *POUMAP_BUILDING;

enum OUMAP_RES_TYPE
{
    OUMRT_TILE,
    OUMRT_BUILDING,

    OUMRT_MAX
};

class OUMapTester
{
public:
    OUMapTester(void);
    virtual ~OUMapTester(void);

    /** 载入地图 */
    bool                        LoadMap(const char* mapname);

    /** 获取地图信息 */
    OUPOINT                     GetSize();
    OUPOINT                     GetTrueSize();
    OUPOINT                     GetTruePosOfScreen();

    void                        MoveArea(DWORD dwTrueX, DWORD dwTrueY, float fDelta);

    OUMAP_CELL                  GetCell(int x, int y)
    {
        Assert((y * m_dwWidth + x) < m_MapCell.size(), "下标越界。");

        return m_MapCell[y * m_dwWidth + x];
    }

    hgeSprite*                  GetMapSprite() { return m_pSprite; }

    /** 初始化和结束 */
    static void                 InitAll();
    static void                 ReleaseAll();

    /** 载入底部Tile资源 */
    static bool                 LoadTile(int id, OUMAP_RES_TYPE type, DWORD transColor = 0);
    /** 显示网格 */
    static void                 SetShowGrid(bool bShow) { m_bShowGrid = bShow; }
    static bool                 GetShowGrid() { return m_bShowGrid; }

protected:
    void                        RebuildTile(float fDelta);

protected:
    static HGE*                 m_pHGE;
    bool                        m_bLoaded;

    DWORD                       m_dwTrueWidth;
    DWORD                       m_dwTrueHeight;
    DWORD                       m_dwWidth;
    DWORD                       m_dwHeight;

    DWORD                       m_dwScreenWidth;
    DWORD                       m_dwScreenHeight;

    int                         m_nTop;
    int                         m_nLeft;

    HTARGET                     m_hTarget;
    hgeSprite*                  m_pSprite;

    /** 地图元素 */
    vector<OUMAP_CELL>          m_MapCell;

    set<int>                    m_TileSet;
    set<int>                    m_BuildingSet;

    static map<int, OUMAP_TEX_REF> m_Tiles;
    static map<int, OUMAP_TEX_REF> m_Buildings;

    static bool                 m_bShowGrid;
};
#endif
