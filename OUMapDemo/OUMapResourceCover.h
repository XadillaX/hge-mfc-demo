#ifndef __OUMAPRESOURCECOVER_H__
#define __OUMAPRESOURCECOVER_H__
#pragma once
#include "oumapresourcetile.h"

#define OUMAP_RES_PATH_COVER "tile\\building\\building"

class OUMapResourceCover : public OUMapResourceTile
{
public:
    OUMapResourceCover(void);
    virtual ~OUMapResourceCover(void);

    virtual bool                Load(DWORD dwID);

protected:
    const char*                 MakeFilePath(DWORD dwID, const char* szExt);
};
#endif
