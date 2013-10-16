#include "StdAfx.h"
#include "OUMapResourceCover.h"

OUMapResourceCover::OUMapResourceCover(void)
{
}

OUMapResourceCover::~OUMapResourceCover(void)
{
}

const char* OUMapResourceCover::MakeFilePath(DWORD dwID, const char* szExt)
{
    static char filename[256];

    sprintf(filename, "%s%dc%s", OUMAP_RES_PATH_COVER, dwID, szExt);
    return filename;
}

bool OUMapResourceCover::Load(DWORD dwID)
{
    bool res = OUMapResourceTile::Load(dwID);
    if(!res) return false;

    this->m_pImg->SetColor(0x9fffffff);
    return true;
}
