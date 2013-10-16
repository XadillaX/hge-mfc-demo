#include "StdAfx.h"
#include "OUMapResourceMgr.h"

OUMapResourceMgr::OUMapResourceMgr(void)
{
}

OUMapResourceMgr::~OUMapResourceMgr(void)
{
}

OUMapResourceObject* OUMapResourceMgr::GetImage(OUMAP_RES_TYPE type, int id)
{
    if(m_ResMap[type].find(id) != m_ResMap[type].end())
    {
        return m_ResMap[type][id];
    }

    if(!LoadRes(type, id)) return NULL;
    
    return m_ResMap[type][id];
}

OUMapResourceTile* OUMapResourceMgr::GetTileImage(int id)
{
    OUMapResourceTile* res = (OUMapResourceTile*)GetImage(OUMRT_TILE, id);
    return res;
}

OUMapResourceCarpet* OUMapResourceMgr::GetCarpetImage(int id)
{
    OUMapResourceCarpet* res = (OUMapResourceCarpet*)GetImage(OUMRT_CARPET, id);
    return res;
}

bool OUMapResourceMgr::LoadRes(OUMAP_RES_TYPE type, int id)
{
    switch(type)
    {
    case OUMAP_RES_TYPE::OUMRT_TILE:
        {
            OUMapResourceTile* tile = new OUMapResourceTile();
            if(!tile->Load(id))
            {
                delete tile;
                return false;
            }

            m_ResMap[type][id] = tile;
            return true;

            break;
        }

    case OUMAP_RES_TYPE::OUMRT_CARPET:
        {
            OUMapResourceCarpet* tile = new OUMapResourceCarpet();
            if(!tile->Load(id))
            {
                delete tile;
                return false;
            }

            m_ResMap[type][id] = tile;
            return true;

            break;
        }

    default: return false;
    }

    return false;
}
