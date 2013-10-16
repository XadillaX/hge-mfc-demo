#include "StdAfx.h"
#include "OUMapReader.h"

OUMapReader::OUMapReader(void) :
    m_bOpened(false),
    m_pHGE(NULL),
    m_dwWidth(0),
    m_dwHeight(0),
    m_dwTileWidth(0),
    m_dwTileHeight(0),
    m_dwObjectCount(0),
    m_dwBuildingCount(0),
    m_dwTableCount(0)
{
    m_pHGE = hgeCreate(HGE_VERSION);

    ASSERT(m_pHGE != NULL);
}

OUMapReader::~OUMapReader(void)
{
    Close();
    m_pHGE->Release();
}

bool OUMapReader::Open(const char* filename)
{
    Close();

    m_pHGE->System_Log("Opening map file [%s].", filename);
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        m_pHGE->System_Log("Failed opening.");
        return false;
    }

    /** 读取文件头 */
    OUMAPFILEHEADER header;
    fread(&header, sizeof(OUMAPFILEHEADER), 1, fp);
    if(header.m_dwMagicNum != OUOGM_MGC_NUMBER)
    {
        m_pHGE->System_Log("Failed opening [Damaged file header].");
        fclose(fp);
        return false;
    }
    if(header.m_dwHeaderSize != sizeof(OUMAPFILEHEADER))
    {
        m_pHGE->System_Log("Failed opening [Damaged file header].");
        fclose(fp);
        return false;
    }

    /** 文件信息 */
    m_dwWidth = header.m_dwMapWidth;
    m_dwHeight = header.m_dwMapHeight;
    m_dwTileWidth = header.m_dwTileWidth;
    m_dwTileHeight = header.m_dwTileHeight;
    m_dwObjectCount = header.m_dwObjectCount;
    m_szMapName = header.m_szMapName;
    
    m_pHGE->System_Log("");
    m_pHGE->System_Log("============================== %s", filename);
    m_pHGE->System_Log(" + Map name     : %s", header.m_szMapName);
    m_pHGE->System_Log(" + Map size     : %d x %d", header.m_dwMapWidth, header.m_dwMapHeight);
    m_pHGE->System_Log(" + Tile size    : %d x %d", header.m_dwTileWidth, header.m_dwTileHeight);
    m_pHGE->System_Log(" + Object count : %d", header.m_dwObjectCount);

    /** 读取每一个方格 */
    int cellcount = m_dwWidth * m_dwHeight;
    OUMapCellInfo cell;
    for(int i = 0; i < cellcount; i++)
    {
        if(fread(&cell, sizeof(cell), 1, fp) != 1)
        {
            m_pHGE->System_Log("Failed opening [Damaged cell information].");
            fclose(fp);

            Close();
            return false;
        }

        m_CellMat.push_back(cell);
        m_nBottomIDSet.insert(cell.m_dwBottomID);
        m_nCarpetIDSet.insert(cell.m_dwCarpetID);
    }

    /** 读取每一个物件 */
    OUMapBuildingInfo building;
    for(int i = 0; i < m_dwObjectCount; i++)
    {
        if(fread(&building, OUMAP_READ_BUILDING_INFO_SIZE, 1, fp) != 1)
        {
            m_pHGE->System_Log("Failed openiing [Damaged building information].");
            fclose(fp);

            Close();
            return false;
        }

        building.m_pExtendInfo = NULL;
        building.m_pCoverCoor = new vector<OUMapCoor>();

        if(building.m_emType == OUMRT_BUILDING) m_dwBuildingCount++;
        else
        if(building.m_emType == OUMRT_TABLE) m_dwTableCount++;

        m_BuildingInfo.push_back(building);
    }

    m_pHGE->System_Log("   - Buildings  : %d", m_dwBuildingCount);
    m_pHGE->System_Log("   - Tables     : %d", m_dwTableCount);
    m_pHGE->System_Log("");
    m_pHGE->System_Log(" + Tiles used   :");
    for(set<int>::iterator it = m_nBottomIDSet.begin(); it != m_nBottomIDSet.end(); it++)
    {
        if(*it == 0) continue;
        m_pHGE->System_Log(
                       "   - 0x%.8X", (*it)
                       );
    }
    m_pHGE->System_Log("");
    m_pHGE->System_Log(" + Carpets used :");
    for(set<int>::iterator it = m_nCarpetIDSet.begin(); it != m_nCarpetIDSet.end(); it++)
    {
        if(*it == 0) continue;
        m_pHGE->System_Log(
                       "   - 0x%.8X", (*it)
                       );
    }

    /** 遮罩层 */
    OUMapCoor coor;
    for(int i = 0; i < m_BuildingInfo.size(); i++)
    {
        if(m_BuildingInfo[i].m_dwID == 5)
        {
            int test = 0;
        }
        for(int j = 0; j < m_BuildingInfo[i].m_dwCoverCount; j++)
        {
            int numread = fread(&coor, sizeof(coor), 1, fp);
            if(numread != 1)
            {
                m_pHGE->System_Log("Failed openiing [Damaged cover information].");
                fclose(fp);

                Close();
                return false;
            }

            m_BuildingInfo[i].m_pCoverCoor->push_back(coor);
        }
    }

    /** 桌子层 */
    OUMapTableInfo table;
    for(int i = 0; i < m_BuildingInfo.size(); i++)
    {
        if(m_BuildingInfo[i].m_emType != OUMRT_TABLE) continue;

        if(fread(&table, sizeof(table), 1, fp) != 1)
        {
            m_pHGE->System_Log("Failed openiing [Damaged table information].");
            fclose(fp);

            Close();
            return false;
        }

        OUMapTableInfo* ptb = new OUMapTableInfo();
        memcpy(ptb, &table, sizeof(table));
        m_BuildingInfo[i].m_pExtendInfo = ptb;
    }

    fclose(fp);
    
    m_bOpened = true;
    m_pHGE->System_Log("==============================");
    m_pHGE->System_Log("Succeed opening.");
    return true;
}

void OUMapReader::Close()
{
    if(!m_bOpened) return;

    m_bOpened = false;

    m_dwWidth = m_dwHeight = 0;
    m_dwTileWidth = m_dwTileHeight = 0;
    m_dwObjectCount = m_dwBuildingCount = m_dwTableCount = 0;
    m_szMapName = "";

    m_CellMat.clear();
    m_nBottomIDSet.clear();
    m_nCarpetIDSet.clear();

    for(int i = 0; i < m_BuildingInfo.size(); i++)
    {
        m_BuildingInfo[i].m_pCoverCoor->clear();
        delete m_BuildingInfo[i].m_pCoverCoor;
    }
    m_BuildingInfo.clear();
}

bool OUMapReader::IsOpened()
{
    return m_bOpened;
}

int OUMapReader::GetCellIdx(int x, int y)
{
    if(x < 0) return 0;
    if(y < 0) return 0;
    if(x >= m_dwWidth) return 0;
    if(y >= m_dwHeight) return 0;

    return x + y * m_dwWidth;
}
