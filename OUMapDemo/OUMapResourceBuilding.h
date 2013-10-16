#ifndef __OUMAPRESOURCEBUILDING_H__
#define __OUMAPRESOURCEBUILDING_H__
#pragma once
#include "oumapresourcetile.h"

#define OUMAP_RES_PATH_BUILDING "tile\\building\\building"

class OUMapResourceBuilding : public OUMapResourceTile
{
public:
    OUMapResourceBuilding(void);
    virtual ~OUMapResourceBuilding(void);

protected:
    const char*                 MakeFilePath(DWORD dwID, const char* szExt);
};

#endif
