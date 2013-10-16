#include "StdAfx.h"
#include "OUMapAStar.h"
#include "OUMapView.h"

bool operator < (OUMapAStarCoor a, OUMapAStarCoor b)
{
    if(a.y < b.y) return true;
    if(a.y > b.y) return false;
    return a.x < b.x;
}

OUMapAStar::OUMapAStar(void) :
    m_nWidth(0),
    m_nHeight(0),
    m_bFound(false)
{
    //memset(m_bBlock, 0, sizeof(m_bBlock));
}

OUMapAStar::~OUMapAStar(void)
{
}

void OUMapAStar::SetMap(int width, int height, OUMapView* map)
{
    m_bFound = false;
    m_nWidth = width;
    m_nHeight = height;
    //memcpy(m_bBlock, block, sizeof(m_bBlock));
    m_pMap = map;

    m_NodeMap.clear();
    m_OpenList.clear();
    m_OpenSet.clear();
    m_CloseSet.clear();
    m_Path.clear();

    m_nTargetX = m_nTargetY = 0;
}


bool OUMapAStar::FindPath(int x1, int y1, int x2, int y2)
{
    m_NodeMap.clear();
    m_OpenList.clear();
    m_OpenSet.clear();
    m_CloseSet.clear();
    m_Path.clear();
    m_bFound = false;

    OUMapAStarMapNode node;
    node.G = 0;
    node.H = GetManhattanDistance(x1, y1, x2, y2);
    node.F = node.H;
    node.parent = NULL;
    node.x = x1;
    node.y = y1;

    m_nTargetX = x2;
    m_nTargetY = y2;
    m_nSourceX = x1;
    m_nSourceY = y1;

    OUMapAStarCoor op;
    op.x = x1;
    op.y = y1;

    m_NodeMap[op] = node;
    m_OpenSet.insert(op);
    m_OpenList.push_back(op);

    return AStar();
}

int OUMapAStar::GetManhattanDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

map<OUMapAStarCoor, OUMapAStarMapNode>* __astarmap = NULL;

bool __fsort(OUMapAStarCoor a, OUMapAStarCoor b)
{
    VERIFY(__astarmap != NULL);

    static OUMapAStarMapNode n1, n2;
    n1 = (*__astarmap)[a];
    n2 = (*__astarmap)[b];

    if(n1.F > n2.F) return true;
    else return false;
}

bool OUMapAStar::CheckCoor(int x, int y)
{
    if(x < 0 || y < 0) return false;
    if(x >= m_nWidth || y >= m_nHeight) return false;
    if(m_pMap->m_bBlockArray[y][x]) return false;
    return true;
}

bool OUMapAStar::AStar()
{
    printf("Find path: \n");
    OUMapAStarCoor dc[][8] = {
        { { 0, -2 }, { 0, 2 }, { -1, 0 }, { 1, 0 }, { 0, -1 }, { 1, -1 }, { 0, 1 }, { 1, 1 } },
        { { 0, -2 }, { 0, 2 }, { -1, 0 }, { 1, 0 }, { -1, -1 }, { 0, -1 }, { -1, 1 }, { 0, 1 } }
    };
    int dirdis[] = { 141, 141, 141, 141, 100, 100, 100, 100 };

    int times = 0;
    OUMapAStarCoor newnode;
    OUMapAStarMapNode newv;
    while(!m_OpenList.empty())
    {
        times++;
        __astarmap = &m_NodeMap;
        sort(m_OpenList.begin(), m_OpenList.end(), __fsort);
        __astarmap = NULL;

        OUMapAStarCoor node = m_OpenList.back();
        m_OpenList.pop_back();
        //OUMapAStarCoor node = m_OpenList.front();
        //m_OpenList.erase(m_OpenList.begin());
        
        m_OpenSet.erase(m_OpenSet.find(node));
        m_CloseSet.insert(node);

        int dcidx = (node.y % 2) ^ 1;
        for(int i = 0; i < 8; i++)
        {
            newnode.x = node.x + dc[dcidx][i].x;
            newnode.y = node.y + dc[dcidx][i].y;

            if(!CheckCoor(newnode.x, newnode.y)) continue;
            if(m_CloseSet.find(newnode) != m_CloseSet.end()) continue;
            if(m_OpenSet.find(newnode) != m_OpenSet.end())
            {
                newv = m_NodeMap[newnode];
                newv.parent = &(m_NodeMap[node]);
                newv.G = newv.parent->G + dirdis[i];
                newv.F = newv.G + newv.H;

                if(m_NodeMap[newnode].F > newv.F)
                {
                    m_NodeMap[newnode] = newv;
                }
            }
            else
            {
                newv.x = newnode.x;
                newv.y = newnode.y;
                newv.parent = &(m_NodeMap[node]);
                newv.G = newv.parent->G + dirdis[i];
                newv.H = GetManhattanDistance(newv.x, newv.y, m_nTargetX, m_nTargetY);
                newv.F = newv.G + newv.H;

                m_NodeMap[newnode] = newv;

                m_OpenSet.insert(newnode);
                m_OpenList.push_back(newnode);
            }

            if(newnode.x == m_nTargetX && newnode.y == m_nTargetY)
            {
                printf("%d time(s), ", times);
                MakePath();
                m_bFound = true;

                return true;
            }
        }
    }

    printf("%d time(s), Impossible.\n", times);
    m_bFound = false;
    return false;
}

void OUMapAStar::MakePath()
{
    OUMapAStarCoor coor;
    coor.x = m_nTargetX;
    coor.y = m_nTargetY;
    m_Path.push_front(coor);

    printf("(%d, %d)", coor.x, coor.y);
    while(coor.x != m_nSourceX || coor.y != m_nSourceY)
    {
        OUMapAStarMapNode node = m_NodeMap[coor];
        coor.x = node.parent->x;
        coor.y = node.parent->y;
        m_Path.push_front(coor);

        printf(" -> (%d, %d)", coor.x, coor.y);
    }
    printf("\n");

    //if(!m_Path.empty())
    //{
    //    m_Path.pop_back();
    //}
}

bool OUMapAStar::GetNextCoor(OUMapAStarCoor* coor)
{
    if(!m_bFound) return false;
    if(m_Path.empty())
    {
        m_bFound = false;
        return false;
    }

    *coor = m_Path.front();
    m_Path.pop_front();

    return true;
}
