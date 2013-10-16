#ifndef __OUMAPASTAR_H__
#define __OUMAPASTAR_H__
#pragma once
#include "global.h"
#include <vector>
#include <map>
#include <list>
#include <set>
#include <algorithm>
using namespace std;

/** F = G + H. */
struct OUMapAStarMapNode
{
    int                         x;
    int                         y;

    int                         G;
    int                         H;
    int                         F;

    OUMapAStarMapNode*          parent;
};

struct OUMapAStarCoor
{
    int                         x;
    int                         y;

    inline bool operator == (OUMapAStarCoor coor)
    {
        return coor.x == x && coor.y == y;
    }
};

class OUMapView;

class OUMapAStar
{
public:
    OUMapAStar(void);
    virtual ~OUMapAStar(void);

    void                        SetMap(int width, int height, OUMapView* map);
    bool                        FindPath(int x1, int y1, int x2, int y2);

    static inline int           GetManhattanDistance(int x1, int y1, int x2, int y2);
    bool                        CheckCoor(int x, int y);

    bool                        GetNextCoor(OUMapAStarCoor* coor);

protected:
    bool                        AStar();
    void                        MakePath();

protected:
    //bool                        m_bBlock[OUMAP_MAX_HEIGHT][OUMAP_MAX_WIDTH];
    OUMapView*                  m_pMap;
    int                         m_nWidth;
    int                         m_nHeight;

    bool                        m_bFound;

    int                         m_nSourceX;
    int                         m_nSourceY;
    int                         m_nTargetX;
    int                         m_nTargetY;

    map<OUMapAStarCoor, OUMapAStarMapNode> m_NodeMap;

    set<OUMapAStarCoor>         m_CloseSet;
    set<OUMapAStarCoor>         m_OpenSet;
    vector<OUMapAStarCoor>      m_OpenList;

    list<OUMapAStarCoor>        m_Path;
};
#endif
