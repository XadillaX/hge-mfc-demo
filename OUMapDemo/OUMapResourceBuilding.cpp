#include "StdAfx.h"
#include "OUMapResourceBuilding.h"

OUMapResourceBuilding::OUMapResourceBuilding(void)
{
}

OUMapResourceBuilding::~OUMapResourceBuilding(void)
{
}

const char* OUMapResourceBuilding::MakeFilePath(DWORD dwID, const char* szExt)
{
    static char filename[256];

    sprintf(filename, "%s%d%s", OUMAP_RES_PATH_BUILDING, dwID, szExt);
    return filename;
}
