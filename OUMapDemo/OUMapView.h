#ifndef __OUMAPVIEW_H__
#define __OUMAPVIEW_H__
#pragma once
#include "Global.h"
#include "OUMapReader.h"
#include "OUMapResourceMgr.h"
#include "OUMapItemBuilding.h"
#include "OUMapItemTable.h"
#include "OUBaseRole.h"
#include "OUMapAStar.h"

#include <vector>
using namespace std;

class OUAITestRole;

class OUMapView
{
friend class OUAITestRole;
friend class OUMapAStar;

public:
    OUMapView();
    virtual ~OUMapView(void);

    bool                    SetMap(const char* filename);
    void                    SetMyRole(OUBaseRole* role)                     { m_pMyRole = role; }
    OUBaseRole*             GetMyRole()                                     { return m_pMyRole; }

    /** 测试功能，请自行移植完善 */
    void                    AddOtherRole(OUBaseRole* role)                  { m_pRoles.push_back(role); }

    void                    SetMapScreen(hgeRect& rect);
    hgeRect                 GetMapRect();

    /** 相对坐标 */
    void                    Update(float fMX, float fMY, float fDelta);
    bool                    RebuildTarget(float fDeltaTime);
    void                    DrawSingleGrid(int sx, int sy, DWORD color);

    /** 重新获取双重缓冲 */
    void                    RestoreTarget();

    hgeSprite*              GetSprite();

    void                    MoveTo(float fLeft, float fTop);
    void                    MoveBy(float x, float y);

    void                    Clear();

    OUMapAStar*             GetAStar()                                      { return &m_AStar; }
    OUMapAStarCoor          GetCoorByTrueWorld(float fX, float fY);
    OUPOINT                 GetCenterOfCoor(int x, int y);
    inline bool             IsBlock(int x, int y)                           { return m_bBlockArray[y][x]; }

    inline int              GetWidth()                                      { return m_nWidth; }
    inline int              GetHeight()                                     { return m_nHeight; }

protected:
    void                    ReaderToThis();

    bool                    CreateTargetAndSprite();
    void                    ReleaseTargetAndSprite();

    void                    RerenderTileLayer(float fDelta);
    void                    RerenderCarpetLayer(float fDelta);
    void                    RerenderBuildingLayer(float fDelta);
    void                    RerenderRoleAndCover(float fDelta);

private:
    HGE*                    m_pHGE;

    OUMapReader*            m_pReader;
    OUMapResourceMgr*       m_pResMgr;

    HTARGET                 m_hMapTarget;
    hgeSprite*              m_pMapSprite;

    hgeRect                 m_rtMapScreen;

    int                     m_nTileArray[OUMAP_MAX_HEIGHT][OUMAP_MAX_WIDTH];
    int                     m_nCarpetArray[OUMAP_MAX_HEIGHT][OUMAP_MAX_WIDTH];
    bool                    m_bBlockArray[OUMAP_MAX_HEIGHT][OUMAP_MAX_WIDTH];
    vector<OUMapItemBuilding*> m_pBuildings;

    /** 左上角相对于地图位置 */
    float                   m_fWorldLeft;
    float                   m_fWorldTop;

    int                     m_nWidth;
    int                     m_nHeight;

    OUBaseRole*             m_pMyRole;
    vector<OUBaseRole*>     m_pRoles;
    OUMapAStar              m_AStar;
};

#endif
