#ifndef __OUMAPREADER_H__
#define __OUMAPREADER_H__
#pragma once
#include "global.h"
#include <string>
#include <set>
#include <vector>
using namespace std;

struct OUMapCellInfo
{
    DWORD                       m_dwBottomID;
    DWORD                       m_dwCarpetID;
    bool                        m_bBlock;
};

struct OUMapTableInfo
{
    int                     m_nID;
    int                     m_nImgID;

    /** 预留补缺 */
    int                     m_nReserved[3];
};

struct OUMapBuildingInfo
{
    DWORD                       m_Order;
    OUMAP_RES_TYPE              m_emType;

    DWORD                       m_dwID;

    float                       m_fTrueX;
    float                       m_fTrueY;

    DWORD                       m_dwCoverCount;

    vector<OUMapCoor>*          m_pCoverCoor;
    LPVOID                      m_pExtendInfo;
};

#define OUMAP_READ_BUILDING_INFO_SIZE (sizeof(OUMapBuildingInfo) - sizeof(vector<OUMapCoor>*) - sizeof(LPVOID))

class OUMapReader
{
public:
    OUMapReader(void);
    ~OUMapReader(void);

    bool                        Open(const char* filename);
    void                        Close();

    bool                        IsOpened();

    inline int                  GetWidth()                      { return m_dwWidth; }
    inline int                  GetHeight()                     { return m_dwHeight; }
    inline int                  GetTileWidth()                  { return m_dwTileWidth; }
    inline int                  GetTileHeight()                 { return m_dwTileHeight; }
    inline int                  GetHalfTileWidth()              { return GetTileWidth() >> 1; }
    inline int                  GetHalfTileHeight()             { return GetTileHeight() >> 1; }

    inline int                  GetWorldWidth()                 { return GetHalfTileWidth() + (m_dwWidth * m_dwTileWidth); }
    inline int                  GetWorldHeight()                { return GetHalfTileHeight() + (m_dwHeight * GetHalfTileHeight()); }

    int                         GetCellIdx(int x, int y);
    OUMapCellInfo               GetCellInfo(int x, int y)       { VERIFY(m_CellMat.size()); return m_CellMat[GetCellIdx(x, y)]; }

    int                         GetBuildingCount()              { return m_BuildingInfo.size(); }
    OUMapBuildingInfo           GetBuildingInfo(int idx)        { VERIFY(m_BuildingInfo.size() > idx); return m_BuildingInfo[idx]; }

private:
    /** 数组 */
    vector<OUMapCellInfo>       m_CellMat;
    vector<OUMapBuildingInfo>   m_BuildingInfo;
    set<int>                    m_nBottomIDSet;
    set<int>                    m_nCarpetIDSet;

    bool                        m_bOpened;
    HGE*                        m_pHGE;

    /** 宽高 */
    DWORD                       m_dwWidth;
    DWORD                       m_dwHeight;

    /** 贴图宽高 */
    DWORD                       m_dwTileWidth;
    DWORD                       m_dwTileHeight;

    /** 物体数（包括建筑数和桌子数） */
    DWORD                       m_dwObjectCount;
    DWORD                       m_dwBuildingCount;
    DWORD                       m_dwTableCount;

    /** 地图名 */
    string                      m_szMapName;
};

#endif
