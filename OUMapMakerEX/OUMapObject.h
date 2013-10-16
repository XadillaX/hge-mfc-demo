#ifndef __OUMAPOBJECT_H__
#define __OUMAPOBJECT_H__

#pragma once

enum OUMAP_OBJECT_TYPE
{
    OMOT_BUILDING,
    OMOT_ROLE,
    OMOT_TABLE,

    OMOT_MAX
};

class OUMapObject
{
public:
    OUMapObject(void);
    virtual ~OUMapObject(void);

    OUMAP_OBJECT_TYPE           GetType()
    {
        return m_emObjectType;
    }

protected:
    OUMAP_OBJECT_TYPE           m_emObjectType;
};

typedef struct tagOUMAP_TILE_COOR
{
    int x, y;
} OUMAP_TILE_COOR, *POUMAP_TILE_COOR;

extern bool operator < (tagOUMAP_TILE_COOR a, tagOUMAP_TILE_COOR c);

#endif
