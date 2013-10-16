#ifndef __OUMAPITEMTABLE_H__
#define __OUMAPITEMTABLE_H__
#pragma once
#include "oumapitembuilding.h"
#include "oumapresourcetable.h"

class OUMapItemTable : public OUMapItemBuilding
{
public:
    OUMapItemTable(void);
    virtual ~OUMapItemTable(void);

    virtual void                ResponseInput(float itemX, float itemY);
    virtual bool                SetBuildingInfo(OUMapBuildingInfo& info);
    virtual void                Render(float fStartX, float fStartY);

    inline int                  GetTableID()                        { return m_nTableID; }

protected:
    int                         m_nTableID;
};

#endif
