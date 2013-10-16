#ifndef __OUMAPITEMBUILDING_H__
#define __OUMAPITEMBUILDING_H__
#pragma once
#include "oumapreader.h"
#include "oumapitemobject.h"
#include <set>
#include "oumapresourcecover.h"
using namespace std;

class OUMapItemBuilding : public OUMapItemObject
{
public:
    OUMapItemBuilding(void);
    virtual ~OUMapItemBuilding(void);

    virtual bool                SetBuildingInfo(OUMapBuildingInfo& info);

    virtual void                Update(float fDelta);
    virtual void                Render(float fStartX, float fStartY);
    virtual void                RenderCover(float fStartX, float fStartY);

    virtual void                SetZ(float z);
    virtual float               GetZ();

    int                         GetResourceID();

    int                         GetOrder()
    {
        return m_nOrder;
    }

    bool                        Covered(int x, int y)
    {
        OUMapCoor coor = { x, y };
        return m_CoverInfo.find(coor) != m_CoverInfo.end();
    }

    OUMapCoor                   GetMaxCoverCoor()
    {
        OUMapCoor coor = { -1, -1 };
        if(!m_CoverInfo.empty())
        {
            set<OUMapCoor>::reverse_iterator it = m_CoverInfo.rbegin();
            return *it;
        }
        else return coor;
    }

    OUMapCoor                   GetMinCoverCoor()
    {
        OUMapCoor coor = { -1, -1 };
        if(!m_CoverInfo.empty())
        {
            set<OUMapCoor>::iterator it = m_CoverInfo.begin();
            return *it;
        }
        else return coor;
    }

protected:
    int                         m_nResID;
    int                         m_nOrder;
    OUMapResourceCover*         m_pCover;


    set<OUMapCoor>              m_CoverInfo;
};

#endif
