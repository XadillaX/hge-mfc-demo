#include "StdAfx.h"
#include "OUMapTester.h"

const string __OUMAPTILESUFFIX[] = { ".oga", ".png", ".jpg", ".tga", ".bmp" };
const int __OUMAPTILESUFFIX_COUNT = 5;

map<int, OUMAP_TEX_REF> OUMapTester::m_Tiles;
map<int, OUMAP_TEX_REF> OUMapTester::m_Buildings;
HGE* OUMapTester::m_pHGE = NULL;
bool OUMapTester::m_bShowGrid = false;

void OUMapTester::InitAll()
{
    m_pHGE = hgeCreate(HGE_VERSION);
}

void OUMapTester::ReleaseAll()
{
    m_pHGE->Release();
}

OUMapTester::OUMapTester(void)
{
    m_bLoaded = false;

    m_dwWidth = m_dwHeight = 0;
    m_dwTrueWidth = m_dwTrueHeight = 0;

    m_nTop = 0;
    m_nLeft = 0;

    m_dwScreenWidth = m_pHGE->System_GetState(HGE_SCREENWIDTH);
    m_dwScreenHeight = 450;

    m_hTarget = m_pHGE->Target_Create(m_dwScreenWidth, m_dwScreenHeight, true);
    m_pSprite = new hgeSprite(m_pHGE->Target_GetTexture(m_hTarget), 0, 0, m_dwScreenWidth, m_dwScreenHeight);
}

OUMapTester::~OUMapTester(void)
{
    m_pHGE->Texture_Free(m_hTarget);

    /** ��ͼ����Դɾ�� */
    for(set<int>::iterator it = m_TileSet.begin(); it != m_TileSet.end(); it++)
    {
        m_Tiles[*it].time--;
        if(m_Tiles[*it].time == 0)
        {
            m_Tiles[*it].img.Release();
            m_pHGE->Texture_Free(m_Tiles[*it].tex);
            m_Tiles.erase(m_Tiles.find(*it));
        }
    }
    m_TileSet.clear();

    /** ������ͼ����Դɾ�� */
    for(set<int>::iterator it = m_BuildingSet.begin(); it != m_BuildingSet.end(); it++)
    {
        m_Buildings[*it].time--;
        if(m_Buildings[*it].time == 0)
        {
            m_Buildings[*it].img.Release();
            m_pHGE->Texture_Free(m_Buildings[*it].tex);
            m_Buildings.erase(m_Buildings.find(*it));
        }
    }
    m_Buildings.clear();

    /** �ͷ�Cell */
    m_MapCell.clear();
}

bool OUMapTester::LoadMap(const char* mapname)
{
    if(m_bLoaded) return false;

    char filename[256];
    sprintf(filename, OUMAP_MAP_NAME_FORMAT, mapname);

    /** �����ͼ�ļ� */
    FILE* fp = fopen(filename, "r");
    if(fp == NULL) return false;

    /** ��� */
    fscanf(fp, "%lu%lu", &m_dwHeight, &m_dwWidth);
    m_dwTrueHeight = HALF_TILE_HEIGHT + (m_dwHeight * HALF_TILE_HEIGHT);
    m_dwTrueWidth = HALF_TILE_WIDTH + (m_dwWidth * HALF_TILE_WIDTH);

    /** ����ײ� */
    for(int i = 0; i < m_dwHeight; i++)
    {
        for(int j = 0; j < m_dwWidth; j++)
        {
            OUMAP_CELL cell;
            
            /** ��ʼ�� */
            cell.x              = j;
            cell.y              = i;
            cell.block          = false;
            cell.m_BuildingID   = 0;
            cell.m_BottomID     = 0;
            cell.m_pBottomImg   = 0;
            cell.m_pBuildingImg = 0;

            fscanf(fp, "%lu", &cell.m_BottomID);

            /** �鿴��û��������Դ */
            if(cell.m_BottomID && m_TileSet.find(cell.m_BottomID) == m_TileSet.end())
            {
                if(m_Tiles.find(cell.m_BottomID) == m_Tiles.end())
                {
                    OUMapTester::LoadTile(cell.m_BottomID, OUMRT_TILE, 0xffff00ff);
                }

                m_Tiles[cell.m_BottomID].time++;
                m_TileSet.insert(cell.m_BottomID);
            }

            if(cell.m_BottomID) cell.m_pBottomImg = &(m_Tiles[cell.m_BottomID].img);

            m_MapCell.push_back(cell);
        }
    }
    fclose(fp);

    /** ���뽨���� */
    sprintf(filename, OUMAP_BMAP_NAME_FORMAT, mapname);
    fp = fopen(filename, "r");
    if(fp == NULL) return true;
    
    for(int i = 0; i < m_dwHeight; i++)
    {
        for(int j = 0; j < m_dwWidth; j++)
        {
            POUMAP_CELL cell = &m_MapCell[i * m_dwWidth + j];
            fscanf(fp, "%lu", &cell->m_BuildingID);

            /** �鿴��û��������Դ */
            if(cell->m_BuildingID && m_BuildingSet.find(cell->m_BuildingID) == m_BuildingSet.end())
            {
                if(m_Buildings.find(cell->m_BuildingID) == m_Buildings.end())
                {
                    OUMapTester::LoadTile(cell->m_BuildingID, OUMRT_BUILDING, 0xffff00ff);
                }

                m_Buildings[cell->m_BuildingID].time++;
                m_BuildingSet.insert(cell->m_BuildingID);
            }
            //else cell->m_BuildingID = 0, cell->m_pBuildingImg = NULL;

            if(cell->m_BuildingID) cell->m_pBuildingImg = &(m_Buildings[cell->m_BuildingID].img);
        }
    }

    fclose(fp);

    return true;
}

bool OUMapTester::LoadTile(int id, OUMAP_RES_TYPE type, DWORD transColor)
{
    /** ��Դ���� */
    map<int, OUMAP_TEX_REF>* pContainer;
    static char prefix[256];
    switch(type)
    {
    case OUMRT_TILE:
        {
            pContainer = &OUMapTester::m_Tiles;
            sprintf(prefix, OUMAP_TILE_NAME_FORMAT, id);
            break;
        }

    case OUMRT_BUILDING:
        {
            pContainer = &OUMapTester::m_Buildings;
            sprintf(prefix, OUMAP_BUILDING_NAME_FORMAT, id);
            break;
        }

    default:
        {
            pContainer = &OUMapTester::m_Tiles;
            sprintf(prefix, OUMAP_TILE_NAME_FORMAT, id);
            break;
        }
    }

    /** �����룬���ǲ��ܸ���͸��ɫ�� */
    if(pContainer->find(id) != pContainer->end()) return true;

    /** ���ݸ���׺����ö�ٲ��� */
    for(int i = 0; i < __OUMAPTILESUFFIX_COUNT; i++)
    {
        string filename = prefix + __OUMAPTILESUFFIX[i];

        /** ���Ų�����Դ */
        OUPACKFILEDATA data = OUResourcePool::instance().GetResource(filename);
        if(data.m_dwFileSize == 0) continue;

        /** ����Ƕ����ذ� */
        if(".oga" == __OUMAPTILESUFFIX[i])
        {
            OUMAP_TEX_REF ref;
            ref.id = id;
            ref.img.m_bStatic = false;
            ref.tex = 0;
            ref.time = 0;
            
            /** ������ʧ������� */
            ref.img.m_Img.m_pAnim = new OUAniFile();
            if(!ref.img.m_Img.m_pAnim->LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize))
            {
                continue;
            }

            ref.img.Play();
            if(type == OUMRT_TILE) ref.img.SetZ(0.2);
            else
            if(type == OUMRT_BUILDING) ref.img.SetZ(0.1);
            
            (*pContainer)[id] = ref;

            return true;
        }
        else
        /** �Ƕ��� */
        {
            OUMAP_TEX_REF ref;
            ref.id = id;
            ref.img.m_bStatic = true;
            ref.tex = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize, true);
            ref.time = 0;

            if(ref.tex == 0) continue;

            int iw, ih;
            iw = m_pHGE->Texture_GetWidth(ref.tex);
            ih = m_pHGE->Texture_GetHeight(ref.tex);

            /** ͸��ɫ */
            if(transColor != 0)
            {
                DWORD* mem = m_pHGE->Texture_Lock(ref.tex, true, 0, 0, iw, ih);
                int imax = iw * ih; //OUMAP_TILE_WIDTH * OUMAP_TILE_HEIGHT;
                for(int i = 0; i < imax; i++)
                {
                    if(mem[i] == transColor) mem[i] = 0;
                }
                m_pHGE->Texture_Unlock(ref.tex);
            }

            ref.img.m_Img.m_pSprite = new hgeSprite(ref.tex, 0, 0, iw, ih);
            if(type == OUMRT_TILE) ref.img.SetZ(0.2);
            else
            if(type == OUMRT_BUILDING) ref.img.SetZ(0.1);

            (*pContainer)[id] = ref;

            return true;
        }
    }

    return false;
}

OUPOINT OUMapTester::GetSize()
{
    OUPOINT p;
    p.m_fX = m_dwWidth;
    p.m_fY = m_dwHeight;

    return p;
}

OUPOINT OUMapTester::GetTrueSize()
{
    OUPOINT p;
    p.m_fX = m_dwTrueWidth;
    p.m_fY = m_dwTrueHeight;

    return p;
}

OUPOINT OUMapTester::GetTruePosOfScreen()
{
    OUPOINT p;
    p.m_fX = m_nLeft;
    p.m_fY = m_nTop;

    return p;
}

void OUMapTester::MoveArea(DWORD dwTrueX, DWORD dwTrueY, float fDelta)
{
    /** ������ʾ��ߺ��ұߵļ� */
    if(m_dwTrueWidth + HALF_TILE_WIDTH < m_dwScreenWidth)
    {
        m_nLeft = (m_dwScreenWidth - m_dwTrueWidth) / 2;
    }
    else
    {
        if(dwTrueX < HALF_TILE_WIDTH) dwTrueX = HALF_TILE_WIDTH;
        if(dwTrueX + m_dwScreenWidth + HALF_TILE_WIDTH > m_dwTrueWidth) dwTrueX = m_dwTrueWidth - m_dwScreenWidth - HALF_TILE_WIDTH;

        m_nLeft = dwTrueX;
    }

    /** ������ʾ���µļ� */
    if(m_dwTrueHeight + HALF_TILE_HEIGHT < m_dwScreenHeight)
    {
        m_nLeft = (m_dwScreenHeight - m_dwTrueHeight) / 2;
    }
    else
    {
        if(dwTrueY < HALF_TILE_HEIGHT) dwTrueY = HALF_TILE_HEIGHT;
        if(dwTrueY + m_dwScreenHeight + HALF_TILE_HEIGHT > m_dwTrueHeight) dwTrueY = m_dwTrueHeight - m_dwScreenHeight - HALF_TILE_HEIGHT;

        m_nTop = dwTrueY;
    }

    /** �ع���ͼ��ͼ */
    RebuildTile(fDelta);
}

/**
 *  ��ͼ������
 *            by XadillaX 
 *      /\/\
 *      \/\/\
 *      /\/\/
 *      \/\/
 *
 *    .........
 *   _________
 *  |  __|____|__
 *  |_|__|__|_|__|
 *  | |__|__|_|__|
 *  |____|____|
 *
 */
void OUMapTester::RebuildTile(float fDelta)
{
    m_pHGE->Gfx_BeginScene(m_hTarget);
    m_pHGE->Gfx_Clear(0xff000000);

    /** ��ʼ��ͼ */
    int tx, ty;
    static bool updated[65536];
    static bool bupdated[65536];
    memset(updated, 0, sizeof(updated));
    memset(bupdated, 0, sizeof(bupdated));
    
    /** �����Ͻǿ�ʼ�� */
    for(int i = 0; i < m_dwHeight; i++)
    {
        /** ������ͼ��y��ʼλ�� */
        ty = i * HALF_TILE_HEIGHT;
        
        /** ����ʼλ�ò�������� */
        if(ty + OUMAP_TILE_HEIGHT <= m_nTop) continue;
        
        /** ����ʼλ�ó��������� */
        if(ty >= m_nTop + m_dwScreenHeight) break;
        
        /** ö�ٱ��и�����ͼ */
        for(int j = 0; j < m_dwWidth; j++)
        {
            /** ����ͼxλ�� */
            tx = j * OUMAP_TILE_WIDTH;
            if(i % 2 == 1) tx += HALF_TILE_WIDTH;
            
            /** ����ʼx��������� */
            if(tx + OUMAP_TILE_WIDTH <= m_nLeft) continue;
            
            /** ����ʼλ�ó��������� */
            if(tx >= m_nLeft + m_dwScreenWidth) break;
            
            /** ��ȡ��ͼ��Ϣ */
            OUMAP_CELL cell = GetCell(j, i);
            
            /** ���Ƕ�̬��ͼ����¶��� */
            if(cell.m_pBottomImg != NULL)
            {
                if(!cell.m_pBottomImg->m_bStatic && !updated[cell.m_BottomID])
                {
                    updated[cell.m_BottomID] = true;
                    cell.m_pBottomImg->GetAnimation()->Update(fDelta);
                }
                
                /** ��Ⱦ��ͼ��Ŀ�� */
                cell.m_pBottomImg->Render(tx - m_nLeft, ty - m_nTop);
            }

            if(cell.m_pBuildingImg != NULL)
            {
                if(!cell.m_pBuildingImg->m_bStatic && !bupdated[cell.m_BuildingID])
                {
                    bupdated[cell.m_BuildingID] = true;
                    cell.m_pBuildingImg->GetAnimation()->Update(fDelta);
                }
                
                /** ��Ⱦ��ͼ��Ŀ�� */
                cell.m_pBuildingImg->Render(tx - m_nLeft, ty - m_nTop);
            }

            //m_Tiles[cell.m_BottomID].img.Render(tx - m_nLeft, ty - m_nTop);
            //OUFontLoader::instance().GetFont(string("����"))->Print(tx - m_nLeft + 10, ty - m_nTop + 5, "%d", cell.m_BottomID);
            if(OUMapTester::m_bShowGrid)
            {
                m_pHGE->Gfx_RenderLine(tx - m_nLeft + HALF_TILE_WIDTH, ty - m_nTop, tx - m_nLeft + OUMAP_TILE_WIDTH, ty - m_nTop + HALF_TILE_HEIGHT, 0xff00ff00);
                m_pHGE->Gfx_RenderLine(tx - m_nLeft + OUMAP_TILE_WIDTH, ty - m_nTop + HALF_TILE_HEIGHT, tx - m_nLeft + HALF_TILE_WIDTH, ty - m_nTop + OUMAP_TILE_HEIGHT, 0xff00ff00);
                m_pHGE->Gfx_RenderLine(tx - m_nLeft + HALF_TILE_WIDTH, ty - m_nTop + OUMAP_TILE_HEIGHT, tx - m_nLeft, ty - m_nTop + HALF_TILE_HEIGHT, 0xff00ff00);
                m_pHGE->Gfx_RenderLine(tx - m_nLeft, ty - m_nTop + HALF_TILE_HEIGHT, tx - m_nLeft + HALF_TILE_WIDTH, ty - m_nTop, 0xff00ff00);
            }
        }
    }

    m_pHGE->Gfx_EndScene();
    
    /** ��Ŀ�������������� */
    m_pSprite->SetTexture(m_pHGE->Target_GetTexture(m_hTarget));
}

