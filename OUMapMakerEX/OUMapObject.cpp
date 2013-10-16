#include "StdAfx.h"
#include "OUMapObject.h"

bool operator < (tagOUMAP_TILE_COOR a, tagOUMAP_TILE_COOR c)
{
    if(a.x < c.x) return true;
    if(a.x > c.x) return false;
    return a.y < c.y;
}

OUMapObject::OUMapObject(void)
{
}

OUMapObject::~OUMapObject(void)
{
}
