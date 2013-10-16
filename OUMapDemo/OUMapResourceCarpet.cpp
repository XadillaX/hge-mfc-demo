#include "StdAfx.h"
#include "OUMapResourceCarpet.h"

OUMapResourceCarpet::OUMapResourceCarpet(void)
{
}

OUMapResourceCarpet::~OUMapResourceCarpet(void)
{
}

const char* OUMapResourceCarpet::MakeFilePath(DWORD dwID, const char* szExt)
{
    static char filename[256];

    sprintf(filename, "%s%d%s", OUMAP_RES_PATH_CARPET, dwID, szExt);
    return filename;
}
