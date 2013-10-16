#ifndef __OUMAPRESOURCECARPET_H__
#define __OUMAPRESOURCECARPET_H__
#pragma once
#include "oumapresourcetile.h"

#define OUMAP_RES_PATH_CARPET  "tile\\carpet\\carpet"

class OUMapResourceCarpet : public OUMapResourceTile
{
public:
    OUMapResourceCarpet(void);
    virtual ~OUMapResourceCarpet(void);

protected:
    virtual const char*         MakeFilePath(DWORD dwID, const char* szExt);
};

#endif
