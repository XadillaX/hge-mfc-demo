#include "StdAfx.h"
#include "OUMapView.h"
#include "OUMapMakerEXView.h"
#include "OUMapTable.h"

const string __OUMAPTILESUFFIX[] = { ".oga", ".png", ".jpg", ".tga", ".bmp" };
const int __OUMAPTILESUFFIX_COUNT = 5;

int OUMapView::m_nTileWidth = DEFAULT_TILE_WIDTH;
int OUMapView::m_nTileHeight = DEFAULT_TILE_HEIGHT;
int OUMapView::m_nHalfTileWidth = DEFAULT_TILE_WIDTH >> 1;
int OUMapView::m_nHalfTileHeight = DEFAULT_TILE_HEIGHT >> 1;

POUMAP_UPPER_CELL OUMAP_UPPER_CELL::m_pFocused = NULL;
POUMAP_UPPER_CELL OUMAP_UPPER_CELL::m_pClickFocused = NULL;

OUMapView::OUMapView(void) :
    m_bShowState(true),
    m_dwWidth(0),
    m_dwHeight(0),
    m_dwTrueWidth(0),
    m_dwTrueHeight(0),
    m_hMapTarget(0),
    m_pMapSprite(NULL),
    m_nLeft(0),
    m_nTop(0),
    m_szName(""),
    m_bShowGrid(false),
    m_dwRenderCount(0),
    m_emEditMode(OUMEM_CHOOSE),
    m_pInnerFont(NULL),
    m_bUpperGrid(true)
{
    ::InitializeCriticalSection(&m_MsgQueueCS);
    m_pHGE = hgeCreate(HGE_VERSION);

    /** �������� */
    ::OUAniFile::InitAll();
    ::OUMapTableImg::InitAll();

    /** �����ͼ */
    //OUMAP_TEX_REF black;
    //black.id = 0;
    //black.tex = m_pHGE->Texture_Load("tile\\blacktile.png");
    //if(black.tex != 0)
    //{
    //    black.img.m_Img.m_pSprite = new hgeSprite(black.tex, 0, 0, m_nTileWidth, m_nTileHeight);
    //}
    //m_Tiles[OUMAP_TILE_BLACKID] = black;

    SetScreenSize(m_pHGE->System_GetState(HGE_SCREENWIDTH), m_pHGE->System_GetState(HGE_SCREENHEIGHT));
    
    /** Ŀ���С */
    OM_SIZE_PARAM* param = new OM_SIZE_PARAM();
    param->m_nScreenWidth = m_pHGE->System_GetState(HGE_SCREENWIDTH);
    param->m_nScreenHeight = m_pHGE->System_GetState(HGE_SCREENHEIGHT);
    SendMessage(OM_SIZE, param);

    /** �½���ͼ */
    OM_NEWMAP_PARAM* param2 = new OM_NEWMAP_PARAM;
    strcpy(param2->m_szName, "�ޱ���");
    param2->m_nWidth = DEFAULT_MAP_WIDTH;
    param2->m_nHeight = DEFAULT_MAP_HEIGHT;
    param2->m_nTileWidth = DEFAULT_TILE_WIDTH;
    param2->m_nTileHeight = DEFAULT_TILE_HEIGHT;
    SendMessage(OM_NEWMAP, param2);

    /** ��ǰѡ��ͼ�� */
    m_CurSelTile.nID = 0;
    m_CurSelTile.emType = OUMRT_TILE;
    m_CurSelTile.pImg = NULL;

    /** ���ָ�� */
    m_hCursor[0] = m_pHGE->Texture_Load("assets/cursor/move.png");
    m_hCursor[1] = m_pHGE->Texture_Load("assets/cursor/add.png");
    m_hCursor[2] = m_pHGE->Texture_Load("assets/cursor/fill.png");
    m_hCursor[3] = m_pHGE->Texture_Load("assets/cursor/erase.png");
    m_hCursor[4] = m_pHGE->Texture_Load("assets/cursor/arrow.png");
    m_hCursor[5] = m_pHGE->Texture_Load("assets/cursor/ground.png");
    m_hCursor[6] = m_pHGE->Texture_Load("assets/cursor/block.png");
    for(int i = 0; i < OUMEM_MAX; i++) ASSERT(m_hCursor[i]);
    m_pCursor[0] = new hgeSprite(m_hCursor[0], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);
    m_pCursor[0]->SetHotSpot(13, 1);
    m_pCursor[1] = new hgeSprite(m_hCursor[1], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);
    m_pCursor[1]->SetHotSpot(2, 28);
    m_pCursor[2] = new hgeSprite(m_hCursor[2], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);
    m_pCursor[2]->SetHotSpot(28, 4);
    m_pCursor[3] = new hgeSprite(m_hCursor[3], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);   
    m_pCursor[3]->SetHotSpot(16, 16);
    m_pCursor[4] = new hgeSprite(m_hCursor[4], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);   
    m_pCursor[4]->SetHotSpot(30, 2);
    m_pCursor[5] = new hgeSprite(m_hCursor[5], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);   
    m_pCursor[5]->SetHotSpot(16, 16);
    m_pCursor[6] = new hgeSprite(m_hCursor[6], 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);   
    m_pCursor[6]->SetHotSpot(16, 16);
    for(int i = 0; i < OUMAP_EDIT_MODE::OUMEM_MAX; i++) m_pCursor[i]->SetZ(0.001);

    m_hCantCursor = m_pHGE->Texture_Load("assets/cant.png");
    m_pCantCursor = new hgeSprite(m_hCantCursor, 0, 0, OUMAP_CURSOR_SIZE, OUMAP_CURSOR_SIZE);
    m_pCantCursor->SetHotSpot(OUMAP_CURSOR_SIZE >> 1, OUMAP_CURSOR_SIZE >> 1);

    /** �ڲ����� */
    OM_SETINNERFONT_PARAM* ftParam = new OM_SETINNERFONT_PARAM();
    strcpy(ftParam->m_szFont, g_IniFile.GetString("dxfont", "fontname", "����").c_str());
    ftParam->m_nSize = g_IniFile.GetInt("dxfont", "fontsize", 12);
    ftParam->m_bBold = !!g_IniFile.GetInt("dxfont", "bold", 0);
    ftParam->m_bItalic = !!g_IniFile.GetInt("dxfont", "italic", 0);
    ftParam->m_bAntialias = !!g_IniFile.GetInt("dxfont", "antialias", 0);
    SendMessage(OUMAPVIEWMSG::OM_SETINNERFONT, ftParam);
}

void OUMapView::SetModified(bool flag)
{
    ::g_pViewDlg->GetDocument()->SetModifiedFlag(flag);
}

OUMapView::~OUMapView(void)
{
    CloseMap();

    /** ���ָ�� */
    for(int i = 0; i < OUMEM_MAX; i++)
    {
        delete m_pCursor[i];
        m_pCursor[i] = NULL;
        m_pHGE->Texture_Free(m_hCursor[i]);
        m_hCursor[i] = 0;
    }
    delete m_pCantCursor;
    m_pCantCursor = NULL;
    m_pHGE->Texture_Free(m_hCantCursor);
    m_pCantCursor = 0;

    m_pHGE->Release();
    m_pHGE = NULL;

    ::DeleteCriticalSection(&m_MsgQueueCS);

    /** �������� */
    ::OUMapTableImg::ReleaseAll();
    ::OUAniFile::ReleaseAll();
}

void OUMapView::NewMap(const char* name, int w, int h, int tw, int th)
{
    /** �ر�ԭ�ȵ�ͼ */
    CloseMap();

    /** ��ͼ��� */
    m_nTileWidth = tw;
    m_nTileHeight = th;
    m_nHalfTileWidth = tw >> 1;
    m_nHalfTileHeight = th >> 1;

    ///** �ײ���ͼ����·�� */
    //static char basepath[256];
    //sprintf(basepath, "%dx%d", tw, th);
    //m_szBasePath = basepath;

    /** �����ͼ */
    GetCellImage(OUMAP_TILE_BLACKID, OUMRT_TILE);
    GetCellImage(OUMAP_TILE_FOCUS, OUMRT_TILE);

    /** ��� */
    m_dwWidth = w;
    m_dwHeight = h;
    CalcTrueWorldSize();

    /** ���Ͻ�λ�� */
    m_nLeft = 0;
    m_nTop = 0;

    /** ��ͼ�� */
    m_szName = name;

    /** ��ͼCell */
    OUMAP_CELL cell;
    cell.m_BottomID = OUMAP_TILE_BLACKID;
    cell.m_pBottomImg = GetCellImage(OUMAP_TILE_BLACKID, OUMRT_TILE);
    cell.block = false;
    cell.m_CarpetID = OUMAP_TILE_BLACKID;
    cell.m_pCarpetImg = NULL;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            cell.x = j, cell.y = i;
            m_MapCell.push_back(cell);
        }
    }

    MoveArea(0, 0);

    /** ��ǰѡ�� */
    if(m_CurSelTile.pImg != NULL)
    {
        m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
    }
}

void OUMapView::CloseMap()
{
    /** ��ͼ */
    m_MapCell.clear();
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        delete *it;
    }
    m_BuildingElement.clear();
    //m_CarpetElement.clear();

    /** ���Ŀ�� */
    //ClearTarget();

    /** �صײ���Դ */
    for(map<int, OUMAP_TEX_REF>::iterator it = m_Tiles.begin(); it != m_Tiles.end(); it++)
    {
        it->second.img.Release();
        m_pHGE->Texture_Free(it->second.tex);
    }
    m_Tiles.clear();

    /** ��̺����Դ */
    for(map<int, OUMAP_TEX_REF>::iterator it = m_CarpetRes.begin(); it != m_CarpetRes.end(); it++)
    {
        it->second.img.Release();
        m_pHGE->Texture_Free(it->second.tex);
    }
    m_CarpetRes.clear();

    /** ��������Դ */
    for(map<int, OUMAP_TEX_REF>::iterator it = m_BuildingRes.begin(); it != m_BuildingRes.end(); it++)
    {
        it->second.img.Release();
        m_pHGE->Texture_Free(it->second.tex);
    }
    m_BuildingRes.clear();

    /** ������ */
    m_dwWidth = m_dwHeight = 0;
    CalcTrueWorldSize();

    /** ������� */
    m_szName = "";

    /** ���Ͻ�λ�� */
    m_nLeft = m_nTop = 0;

    /** ��ǰѡ�� */
    //m_CurSelTile.emType = OUMAP_RES_TYPE::OUMRT_TILE;
    //m_CurSelTile.nID = OUMAP_TILE_BLACKID;
    //m_CurSelTile.pImg = NULL;
}

struct OUMAPTABLE_TOSAVE
{
    int                     m_nID;
    int                     m_nImgID;
    list<POUMAP_UPPER_CELL>::iterator m_itTable;
};

bool __tablecmp(OUMAPTABLE_TOSAVE a, OUMAPTABLE_TOSAVE b)
{
    return a.m_nID < b.m_nID;
}

BOOL OUMapView::OpenMap(LPCTSTR szFilename)
{
    FILE* fp = fopen(szFilename, "rb");
    if(NULL == fp) return FALSE;

    CloseMap();

    GetCellImage(OUMAP_TILE_BLACKID, OUMRT_TILE);
    GetCellImage(OUMAP_TILE_FOCUS, OUMRT_TILE);

    /** �ļ�ͷ */
    OUMAP_MAPFILE_HEADER header;
    fread(&header, sizeof(header), 1, fp);
    if(header.m_dwMagicNum != OUMAP_MAGIC_NUMBER) return FALSE;
    if(header.m_dwHeaderSize != sizeof(OUMAP_MAPFILE_HEADER)) return FALSE;

    /** ��� */
    m_dwWidth = header.m_dwMapWidth;
    m_dwHeight = header.m_dwMapHeight;
    m_nTileWidth = header.m_dwTileWidth;
    m_nTileHeight = header.m_dwTileHeight;
    m_nHalfTileWidth = header.m_dwTileWidth >> 1;
    m_nHalfTileHeight = header.m_dwTileHeight >> 1;
    m_szName = header.m_szMapName;

    this->CalcTrueWorldSize();
    
    /** ÿ��ͼ�� */
    int maxcell = m_dwWidth * m_dwHeight;
    OUMAP_CELL_TOSAVE toread;
    OUMAP_CELL tilecell;
    for(int i = 0; i < maxcell; i++)
    {
        fread(&toread, sizeof(OUMAP_CELL_TOSAVE), 1, fp);
        tilecell.block = toread.m_bBlock;
        tilecell.m_BottomID = toread.m_dwBottomID;
        tilecell.m_CarpetID = toread.m_dwCarpetID;
        tilecell.x = maxcell % m_dwWidth;
        tilecell.y = maxcell / m_dwWidth;
        tilecell.m_pBottomImg = this->GetCellImage(tilecell.m_BottomID, OUMAP_RES_TYPE::OUMRT_TILE);
        tilecell.m_pCarpetImg = this->GetCellImage(tilecell.m_CarpetID, OUMAP_RES_TYPE::OUMRT_CARPET);
        
        m_MapCell.push_back(tilecell);
    }

    /** ���������Ϣ */
    OUMAP_UPPER_CELL_TOSAVE utoread;
    POUMAP_UPPER_CELL pbuildingcell;
    vector<int> cnt;
    int tablecnt = 0;
    for(int i = 0; i < header.m_dwObjectCount; i++)
    {
         fread(&utoread, sizeof(OUMAP_UPPER_CELL_TOSAVE), 1, fp);

         /** ���� */
         if(utoread.m_emType == OUMAP_RES_TYPE::OUMRT_BUILDING)
         {
             pbuildingcell = new OUMAP_UPPER_CELL();
             pbuildingcell->m_dwID = utoread.m_dwID;
             pbuildingcell->m_emType = utoread.m_emType;
             pbuildingcell->m_fTrueX = utoread.m_fTrueX;
             pbuildingcell->m_fTrueY = utoread.m_fTrueY;
             pbuildingcell->m_pImg = GetCellImage(pbuildingcell->m_dwID, pbuildingcell->m_emType);

             m_BuildingElement.push_back(pbuildingcell);
             cnt.push_back(utoread.m_dwCoverCount);
         }
         else
         if(utoread.m_emType == OUMAP_RES_TYPE::OUMRT_TABLE)
         {
             OUMapTable* table = new OUMapTable();
             table->m_emType = OUMRT_TABLE;
             table->m_fTrueX = utoread.m_fTrueX;
             table->m_fTrueY = utoread.m_fTrueY;

             tablecnt++;

             m_BuildingElement.push_back(table);
             cnt.push_back(utoread.m_dwCoverCount);
         }
    }

    /** ���ֲ� */
    int i = 0;
    OUMAP_TILE_COOR covercoor;
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++, i++)
    {
        for(int j = 0; j < cnt[i]; j++)
        {
            fread(&covercoor, sizeof(OUMAP_TILE_COOR), 1, fp);
            (*it)->m_pGroundSet->insert(covercoor);
        }
    }

    /** ���Ӳ� */    
    OUMAPTABLE_TOSAVE ttoread[1000];
    char buf[64];
    list<POUMAP_UPPER_CELL>::iterator myit = m_BuildingElement.begin();
    for(int i = 0; i < tablecnt; i++)
    {
        while((*myit)->m_emType != OUMRT_TABLE && myit != m_BuildingElement.end())
        {
            myit++;
        }
        fread(ttoread + i, /** sizeof(OUMAPTABLE_TOSAVE) - sizeof(int) * 3 */sizeof(int) * 2, 1, fp);
        fread(buf, sizeof(int) * 3, 1, fp);
        ttoread[i].m_itTable = myit;

        myit++;
    }
    sort(ttoread, ttoread + tablecnt, __tablecmp);
    for(int i = 0; i < tablecnt; i++)
    {
        float fTrueX = (*ttoread[i].m_itTable)->m_fTrueX;
        float fTrueY = (*ttoread[i].m_itTable)->m_fTrueY;

        delete (*ttoread[i].m_itTable);
        (*ttoread[i].m_itTable) = new OUMapTable();
        (*ttoread[i].m_itTable)->m_emType = OUMRT_TABLE;
        (*ttoread[i].m_itTable)->m_dwID = ttoread[i].m_nImgID;
        (*ttoread[i].m_itTable)->m_fTrueX = fTrueX;
        (*ttoread[i].m_itTable)->m_fTrueY = fTrueY;
        (*ttoread[i].m_itTable)->m_pImg = this->GetCellImage(ttoread[i].m_nImgID, OUMAP_RES_TYPE::OUMRT_TABLE);
    }

    fclose(fp);

    m_nLeft = 0;
    m_nTop = 0;

    MoveArea(m_nLeft, m_nTop, false);

    /** ��ǰѡ�� */
    if(m_CurSelTile.pImg != NULL)
    {
        m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
    }

    return TRUE;
}

BOOL OUMapView::SaveMap(LPCTSTR szFilename)
{
    /** ��ͼ�ṹ */
    /**
     *
     * Header -> ÿ��Tile�Ľṹ -> ÿ���������Ϣ -> ������Ϣ
     *
     */
    FILE* fp = fopen(szFilename, "wb+");
    if(fp == NULL) return FALSE;
    
    /** �ļ�ͷ */
    OUMAP_MAPFILE_HEADER header;
    header.m_dwMagicNum = OUMAP_MAGIC_NUMBER;
    header.m_dwMapHeight = m_dwHeight;
    header.m_dwMapWidth = m_dwWidth;
    header.m_dwTileWidth = m_nTileWidth;
    header.m_dwTileHeight = m_nTileHeight;
    strcpy_s(header.m_szMapName, 64, m_szName.c_str());
    header.m_dwObjectCount = m_BuildingElement.size();
    header.m_dwHeaderSize = sizeof(OUMAP_MAPFILE_HEADER);
    
    fwrite(&header, header.m_dwHeaderSize, 1, fp);

    /** ÿ����Ϣ */
    int cellcount = m_dwWidth * m_dwHeight;
    OUMAP_CELL_TOSAVE tosave;
    for(int i = 0; i < cellcount; i++)
    {
        tosave.m_bBlock = m_MapCell[i].block;
        tosave.m_dwBottomID = m_MapCell[i].m_BottomID;
        tosave.m_dwCarpetID = m_MapCell[i].m_CarpetID;

        fwrite(&tosave, sizeof(tosave), 1, fp);
    }

    /** �����Ϣ */
    OUMAP_UPPER_CELL_TOSAVE utosave;
    int cnt = 0;
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        utosave.m_dwCoverCount = (*it)->m_pGroundSet->size();
        utosave.m_dwID = (*it)->m_dwID;
        utosave.m_emType = (*it)->m_emType;
        utosave.m_fTrueX = (*it)->m_fTrueX;
        utosave.m_fTrueY = (*it)->m_fTrueY;
        utosave.m_Order = cnt++;

        fwrite(&utosave, sizeof(utosave), 1, fp);
    }

    /** ������Ϣ */
    OUMAP_TILE_COOR covercoor;
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        for(set<OUMAP_TILE_COOR>::iterator it2 = (*it)->m_pGroundSet->begin(); it2 != (*it)->m_pGroundSet->end(); it2++)
        {
            covercoor.x = (*it2).x;
            covercoor.y = (*it2).y;

            fwrite(&covercoor, sizeof(covercoor), 1, fp);
        }
    }

    /** ������Ϣ */
    int a[3] = { 0, 0, 0 };
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        if((*it)->GetType() == OUMAP_OBJECT_TYPE::OMOT_TABLE)
        {
            OUMAPTABLE_TOSAVE save;
            save.m_itTable = it;
            save.m_nID = ((OUMapTable*)(*it))->GetTableID();
            save.m_nImgID = (*it)->m_dwID;

            fwrite(&save, /** sizeof(save) - sizeof(int) * 3 */sizeof(int) * 2, 1, fp);
            fwrite(a, sizeof(a), 1, fp);
        }
        else continue;
    }

    fclose(fp);

    return TRUE;
}

void OUMapView::Render()
{
    if(m_pMapSprite && m_hMapTarget)
    {
        m_pMapSprite->SetTexture(m_pHGE->Target_GetTexture(m_hMapTarget));
        m_pMapSprite->Render(0, 0);
    }
    else
    {
        TRACE0("��ͼ����ָ��Ϊ�ա�\n");
    }

    /** ����� */
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);
    if(m_pHGE->Input_IsMouseOver() && (m_emEditMode == OUMEM_ADD || m_emEditMode == OUMEM_FILL))
    {
        /** ��ͼƬ */
        if(m_CurSelTile.pImg)
        {
            switch(m_CurSelTile.emType)
            {
                /** ѡ�еر���ͼ */
            case OUMRT_TILE:
                {
                    if(OUMAP_TILE_BLACKID == m_CurSelTile.nID) break;

                    /** ������Ӱ */
                    if(m_Tiles.find(OUMAP_TILE_BLACKID) != m_Tiles.end())
                    {
                        m_Tiles[OUMAP_TILE_BLACKID].img.RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight + 10, m_nTileWidth, m_nTileHeight);
                    }

                    m_CurSelTile.pImg->RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight, m_nTileWidth, m_nTileHeight);

                    /** ����� */
                    int x = mx - m_nHalfTileWidth;
                    int y = my - m_nHalfTileHeight;
                    this->DrawSingleGrid(x, y, 0xaaaaaa);
                    //m_pHGE->Gfx_RenderLine(x + m_nHalfTileWidth,
                    //    y,
                    //    x + m_nTileWidth,
                    //    y + m_nHalfTileHeight,
                    //    0xffff0000);
                    //m_pHGE->Gfx_RenderLine(x + m_nTileWidth,
                    //    y + m_nHalfTileHeight,
                    //    x + m_nHalfTileWidth, 
                    //    y + m_nTileHeight,
                    //    0xffff0000);
                    //m_pHGE->Gfx_RenderLine(x + m_nHalfTileWidth,
                    //    y + m_nTileHeight, 
                    //    x, 
                    //    y + m_nHalfTileHeight,
                    //    0xffff0000);
                    //m_pHGE->Gfx_RenderLine(x, 
                    //    y + m_nHalfTileHeight, 
                    //    x + m_nHalfTileWidth, 
                    //    y, 
                    //    0xffff0000);

                    break;
                }

                /** ѡ�е�̺��ͼ */
            case OUMRT_CARPET:
                {
                    /** Fill״̬������䡪��������xx */
                    //if(m_emEditMode == OUMEM_FILL)
                    //{
                    //    m_pCantCursor->SetColor(m_pHGE->Input_GetKeyState(HGEK_LBUTTON) ? 0xffff0000 : 0xffffffff);
                    //    m_pCantCursor->Render(mx, my);
                    //    break;
                    //}

                    if(OUMAP_TILE_BLACKID == m_CurSelTile.nID) break;

                    /** ������Ӱ */
                    //if(m_Tiles.find(OUMAP_TILE_BLACKID) != m_Tiles.end())
                    //{
                    //    m_Tiles[OUMAP_TILE_BLACKID].img.RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight + 10, m_nTileWidth, m_nTileHeight);
                    //}

                    m_CurSelTile.pImg->RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight, m_nTileWidth, m_nTileHeight);

                    break;
                }

                /** ѡ�н�����ͼ */
            case OUMRT_BUILDING:
                {
                    /** Fill״̬������䡪��������xx */
                    if(m_emEditMode == OUMEM_FILL)
                    {
                        m_pCantCursor->SetColor(m_pHGE->Input_GetKeyState(HGEK_LBUTTON) ? 0xffff0000 : 0xffffffff);
                        m_pCantCursor->Render(mx, my);
                        break;
                    }

                    if(OUMAP_TILE_BLACKID == m_CurSelTile.nID) break;

                    //m_CurSelTile.pImg->RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight, m_nTileWidth, m_nTileHeight);
                    
                    OUPOINT sz = m_CurSelTile.pImg->GetSize();
                    m_CurSelTile.pImg->Render(mx - sz.m_fX / 2, my - sz.m_fY / 2);

                    break;
                }

                /** ������ͼ */
            case OUMRT_TABLE:
                {
                    /** Fill״̬������䡪��������xx */
                    if(m_emEditMode == OUMEM_FILL)
                    {
                        m_pCantCursor->SetColor(m_pHGE->Input_GetKeyState(HGEK_LBUTTON) ? 0xffff0000 : 0xffffffff);
                        m_pCantCursor->Render(mx, my);
                        break;
                    }

                    if(OUMAP_TILE_BLACKID == m_CurSelTile.nID) break;

                    OUPOINT sz = m_CurSelTile.pImg->GetSize();
                    m_CurSelTile.pImg->Render(mx - sz.m_fX / 2, my - sz.m_fY / 2);

                    break;
                }
            }
        }
    }

    /** �����ָ�� */
    if(m_pHGE->Input_IsMouseOver())
    {
        hgeSprite* spr = m_pCursor[m_emEditMode];
        if(spr)
        {
            spr->Render(mx, my);
        }
    }
}

void OUMapView::OnUpdate(float fDelta)
{
    /** ��Ϣѭ�� */
    static OUMAP_MSG_QUEUE_ITEM Msg;
    while(true)
    {
        ::EnterCriticalSection(&m_MsgQueueCS);
        bool empty = m_MsgQueue.empty();
        if(empty)
        {
            ::LeaveCriticalSection(&m_MsgQueueCS);
            break;
        }
        else
        {
            Msg = m_MsgQueue.front();
            m_MsgQueue.pop();
        }
        ::LeaveCriticalSection(&m_MsgQueueCS);

        TranslateMessage(Msg);
    }

    if(g_bFirstWillOpen)
    {
        g_pViewDlg->GetDocument()->OnOpenDocument(g_szFirstWillOpenName.c_str());
        g_bFirstWillOpen = false;
    }

    static int dirx[4] = { -1, 0, 1, 0 };
    static int diry[4] = { 0, -1, 0, 1 };
    OUPOINT pos = GetTruePosOfScreen();

    /** ���������ƶ����� */
    for(int i = HGEK_LEFT; i <= HGEK_DOWN; i++)
    {
        if(m_pHGE->Input_GetKeyState(i))
        {
            pos.m_fX += (dirx[i - HGEK_LEFT] * 20);
            pos.m_fY += (diry[i - HGEK_LEFT] * 20);
        }   
    }

    /** ������ */
    int nWheel = m_pHGE->Input_GetMouseWheel();
    if(nWheel) pos.m_fY -= (nWheel * 50);
    
    MoveArea(pos.m_fX, pos.m_fY);

    /** �༭ģʽ */
    switch(m_emEditMode)
    {
        /** ѡ��ģʽ */
    case OUMEM_CHOOSE:
        {
            ChooseMapObject();
            break;
        }

        /** �ػ�ģʽ */
    case OUMEM_GROUND:
        {
            DoGround();
            break;
        }

        /** �ƶ�ģʽ */
    case OUMEM_MOVE: MoveMapByMouse(); break;

        /** �ϰ���ģʽ */
    case OUMEM_BLOCK:
        {
            DoBlock();
            break;
        }

        /** ��Ƥ��ģʽ */
    case OUMEM_ERASE:
        {
            /** ��Ƥ����ʱ��ҲҪѡ���������ֻ������break */
            ChooseMapObject();
        }

    default: DoEditMap(); break;
    }

    /** ɾ���� */
    if(m_pHGE->Input_GetKeyState(HGEK_DELETE))
    {
        float mx, my;
        m_pHGE->Input_GetMousePos(&mx, &my);
        float nMouseWorldX = m_nLeft - mx;
        float nMouseWorldY = m_nTop - my;
        OUMAP_UPPER_CELL::m_pClickFocused = GetFirstMapObjectByMouse(nMouseWorldX, nMouseWorldY);

        if(OUMAP_UPPER_CELL::m_pFocused)
        {
            OUMAP_UPPER_CELL cell = *OUMAP_UPPER_CELL::m_pFocused;

            for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
            {
                if((*it) == OUMAP_UPPER_CELL::m_pFocused)
                {
                    m_BuildingElement.erase(it);
                    SetModified();
                    break;
                }
            }

            //m_BuildingElement.push_back(cell);
            OUMAP_UPPER_CELL::m_pFocused = NULL;
        }
    }

    //if(m_emEditMode != OUMAP_EDIT_MODE::OUMEM_MOVE) DoEditMap();
    //else MoveMapByMouse();

    RebuildTarget(fDelta);
}

void OUMapView::DoGround()
{
    if(!m_pHGE->Input_IsMouseOver()) return;

    /** û��ѡ�е���� */
    if(OUMAP_UPPER_CELL::m_pClickFocused == NULL) return;

    POUMAP_UPPER_CELL pCell = OUMAP_UPPER_CELL::m_pClickFocused;
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);

    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + mx;
    int nMouseWorldY = m_nTop + my;

    /** ��ͼ���� */
    POINT coor = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    OUMAP_TILE_COOR MapCoor;
    MapCoor.x = coor.x;
    MapCoor.y = coor.y;

    if(m_pHGE->Input_GetKeyState(HGEK_LBUTTON))
    {
        /** �� */
        pCell->m_pGroundSet->insert(MapCoor);

        /** �����޸� */
        SetModified();
    }
    else
    if(m_pHGE->Input_GetKeyState(HGEK_RBUTTON))
    {
        /** �� */
        set<OUMAP_TILE_COOR>::iterator it = pCell->m_pGroundSet->find(MapCoor);
        if(it != pCell->m_pGroundSet->end())
        {
            pCell->m_pGroundSet->erase(it);
            SetModified();
        }
    }
}

void OUMapView::DoBlock()
{
    if(!m_pHGE->Input_IsMouseOver()) return;
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);

    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + mx;
    int nMouseWorldY = m_nTop + my;

    /** ��ͼ���� */
    POINT coor = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    if(coor.x < 0 || coor.y < 0 || coor.x >= m_dwWidth || coor.y >= m_dwHeight) return;

    POUMAP_CELL pCell = GetCellPtr(coor.x, coor.y);

    if(m_pHGE->Input_GetKeyState(HGEK_LBUTTON))
    {
        /** �� */
        pCell->block = true;
        SetModified();
    }
    else
    if(m_pHGE->Input_GetKeyState(HGEK_RBUTTON))
    {
        /** �� */
        pCell->block = false;
        SetModified();
    }
}

void OUMapView::ChooseMapObject()
{
    if(m_CurSelTile.emType != OUMAP_RES_TYPE::OUMRT_BUILDING && m_CurSelTile.emType != OUMAP_RES_TYPE::OUMRT_TABLE) return;
    if(!m_pHGE->Input_IsMouseOver()) return;

    static bool bLastDown = false;
    bool bLButtonDown = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);
    float mx, my;
    static float lastmx = 0, lastmy = 0;
    m_pHGE->Input_GetMousePos(&mx, &my);

    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + mx;
    int nMouseWorldY = m_nTop + my;

    /** û���¡������ȡ������� */
    if(!bLButtonDown)
    {
        OUMAP_UPPER_CELL::m_pFocused = GetFirstMapObjectByMouse(nMouseWorldX, nMouseWorldY);
    }
    else
    /** �����������²�����ƶ� */
    if(bLButtonDown)
    {
        OUMAP_UPPER_CELL::m_pClickFocused = OUMAP_UPPER_CELL::m_pFocused;

        /** ����ǵ�һ�ΰ��£��򽫸������������ */
        if(!bLastDown)
        {
            OUMAP_UPPER_CELL::m_pFocused = GetFirstMapObjectByMouse(nMouseWorldX, nMouseWorldY);

            if(OUMAP_UPPER_CELL::m_pFocused)
            {
                POUMAP_UPPER_CELL cell = OUMAP_UPPER_CELL::m_pFocused;

                for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
                {
                    if((*it) == OUMAP_UPPER_CELL::m_pFocused)
                    {
                        m_BuildingElement.erase(it);
                        break;
                    }
                }

                m_BuildingElement.push_back(cell);
                OUMAP_UPPER_CELL::m_pFocused = *m_BuildingElement.rbegin();
            }
        }
        else
        /** �����ƶ� */
        {
            if(m_emEditMode == OUMEM_CHOOSE)
            {
                float dx = mx - lastmx;
                float dy = my - lastmy;

                if(OUMAP_UPPER_CELL::m_pClickFocused)
                {
                    OUMAP_UPPER_CELL::m_pClickFocused->m_fTrueX += dx;
                    OUMAP_UPPER_CELL::m_pClickFocused->m_fTrueY += dy;
                    SetModified();
                }
            }
        }
    }

    bLastDown = bLButtonDown;
    lastmx = mx;
    lastmy = my;
}

void OUMapView::DoEditMap()
{
    if(!m_pHGE->Input_IsMouseOver()) return;
    bool bLButtonDown = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);
    //if(!bLButtonDown) return;
    
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);
    switch(m_emEditMode)
    {
        /** �滭ģʽ */
    case OUMEM_ADD:
        {
            AddTile(mx, my);
            break;
        }

        /** ���ģʽ */
    case OUMEM_FILL:
        {
            if(!bLButtonDown) break;

            switch(m_CurSelTile.emType)
            {
            case OUMRT_TILE:
                {
                    FillBottomTile(mx, my);
                    break;
                }

            case OUMRT_CARPET:
                {
                    FillCarpetTile(mx, my);
                    break;
                }

                /** TODO: ������ */

                /** ����㲻������� */
            default: break;
            }

            break;
        }

        /** ��Ƥģʽ */
    case OUMEM_ERASE:
        {
            switch(m_CurSelTile.emType)
            {
            case OUMAP_RES_TYPE::OUMRT_CARPET:
            case OUMAP_RES_TYPE::OUMRT_TILE:
                {
                    if(!bLButtonDown) break;

                    /** ����ǵ�̺��͵صײ� */
                    EraseTile(mx, my);
                    break;
                }

            case OUMAP_RES_TYPE::OUMRT_TABLE:
            case OUMAP_RES_TYPE::OUMRT_BUILDING:
                {
                    /** ����ǽ����� */
                    EraseObject(mx, my);

                    break;
                }

            default: break;
            }

            break;
        }
    }
}

void OUMapView::EraseObject(float fMX, float fMY)
{
    static bool lastdown = false;
    bool bLButtonDown = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);

    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    OUMAP_UPPER_CELL::m_pFocused = GetFirstMapObjectByMouse(nMouseWorldX, nMouseWorldY);

    if(OUMAP_UPPER_CELL::m_pFocused && lastdown && !bLButtonDown)
    {
        //OUMAP_UPPER_CELL cell = *OUMAP_UPPER_CELL::m_pFocused;

        for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
        {
            if((*it) == OUMAP_UPPER_CELL::m_pFocused)
            {
                m_BuildingElement.erase(it);
                SetModified();
                break;
            }
        }

        delete OUMAP_UPPER_CELL::m_pFocused;
        OUMAP_UPPER_CELL::m_pFocused = NULL;
        //m_BuildingElement.push_back(cell);
        //OUMAP_UPPER_CELL::m_pFocused = &(*m_BuildingElement.rbegin());

        ChooseMapObject();
    }

    lastdown = bLButtonDown;
}

void OUMapView::AddTile(float fMX, float fMY)
{
    bool bLButtonDown = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);

    /** ��ǰѡ�� */
    switch(m_CurSelTile.emType)
    {
    case OUMRT_TILE:
        {
            if(!bLButtonDown) break;

            if(m_CurSelTile.nID == OUMAP_TILE_BLACKID) break;
            AddBottomTile(fMX, fMY);
            break;
        }

    case OUMRT_CARPET:
        {
            if(!bLButtonDown) break;

            if(m_CurSelTile.nID == OUMAP_TILE_BLACKID) break;
            AddCarpetTile(fMX, fMY);
            break;
        }

    case OUMRT_BUILDING:
        {
            /** �ڽ�������굯�������ͼ */
            static bool lastok = false;
            if(!bLButtonDown)
            {
                lastok = false;
                break;
            }
            else
            if(lastok) break;

            if(m_CurSelTile.nID == OUMAP_TILE_BLACKID) break;
            AddBuildingTile(fMX, fMY);
            lastok = true;

            break;
        }

    case OUMRT_TABLE:
        {
            static bool lastok = false;
            if(!bLButtonDown)
            {
                lastok = false;
                break;
            }
            else
            if(lastok) break;

            if(m_CurSelTile.nID == OUMAP_TILE_BLACKID) break;
            AddTableTile(fMX, fMY);
            lastok = true;

            break;
        }

        /** TODO: ������ */
    }
}

void OUMapView::AddBottomTile(float fMX, float fMY)
{
    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    if(nCol >= 0 && nRow >= 0 && nCol < m_dwWidth && nRow < m_dwHeight)
    {
        POUMAP_CELL pCell = GetCellPtr(nCol, nRow);
        pCell->m_BottomID = m_CurSelTile.nID;
        pCell->m_pBottomImg = m_CurSelTile.pImg;

        SetModified();
    }
}

void OUMapView::AddCarpetTile(float fMX, float fMY)
{
    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    if(nCol >= 0 && nRow >= 0 && nCol < m_dwWidth && nRow < m_dwHeight)
    {
        POUMAP_CELL pCell = GetCellPtr(nCol, nRow);
        pCell->m_CarpetID = m_CurSelTile.nID;
        pCell->m_pCarpetImg = m_CurSelTile.pImg;

        SetModified();
    }

    //OUMAP_UPPER_CELL cell;
    //cell.m_dwID = m_CurSelTile.nID;
    //cell.m_emType = m_CurSelTile.emType;
    //cell.m_fTrueX = nMouseWorldX;
    //cell.m_fTrueY = nMouseWorldY;
    //cell.m_pImg = m_CurSelTile.pImg;
    //m_CarpetElement.push_back(cell);
}

void OUMapView::AddBuildingTile(float fMX, float fMY)
{
    if(!m_CurSelTile.pImg) return;

    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    OUPOINT sz;
    sz = m_CurSelTile.pImg->GetSize();

    POUMAP_UPPER_CELL cell = new OUMAP_UPPER_CELL();
    cell->m_dwID = m_CurSelTile.nID;
    cell->m_emType = m_CurSelTile.emType;
    cell->m_fTrueX = nMouseWorldX - sz.m_fX / 2;
    cell->m_fTrueY = nMouseWorldY - sz.m_fY / 2;
    cell->m_pImg = m_CurSelTile.pImg;
    
    m_BuildingElement.push_back(cell);

    SetModified();
}

void OUMapView::AddTableTile(float fMX, float fMY)
{
    if(!m_CurSelTile.pImg) return;

    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    OUPOINT sz;
    sz = m_CurSelTile.pImg->GetSize();

    OUMapTable* cell = new OUMapTable();
    cell->m_dwID = m_CurSelTile.nID;
    cell->m_emType = m_CurSelTile.emType;
    cell->m_fTrueX = nMouseWorldX - sz.m_fX / 2;
    cell->m_fTrueY = nMouseWorldY - sz.m_fY / 2;
    cell->m_pImg = m_CurSelTile.pImg;
    
    m_BuildingElement.push_back(cell);

    SetModified();
}

void OUMapView::EraseTile(float fMX, float fMY)
{
    /** ��ǰѡ�� */
    switch(m_CurSelTile.emType)
    {
    case OUMRT_TILE:
        {
            //if(m_CurSelTile.nID == OUMAP_TILE_BLACKID) break;
            EraseBottomTile(fMX, fMY);
            break;
        }

        /** TODO: ������ */
    }
}

void OUMapView::EraseBottomTile(float fMX, float fMY)
{
    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    if(nCol >= 0 && nRow >= 0 && nCol < m_dwWidth && nRow < m_dwHeight)
    {
        POUMAP_CELL pCell = GetCellPtr(nCol, nRow);
        pCell->m_BottomID = OUMAP_TILE_BLACKID;
        pCell->m_pBottomImg = this->GetCellImage(OUMAP_TILE_BLACKID, OUMAP_RES_TYPE::OUMRT_TILE);

        SetModified();
    }
}

#include "oumapmakerex.h"
void OUMapView::FillBottomTile(float fMX, float fMY)
{
    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    /** ��֤�Ϸ��� */
    if(nCol < 0 || nRow < 0) return;
    if(nCol >= m_dwWidth || nRow >= m_dwHeight) return;

    /** ��ʼFloodFill */
    bool* bVis = new bool[m_dwWidth * m_dwHeight];
    memset(bVis, 0, sizeof(bool) * m_dwWidth * m_dwHeight);
    POUMAP_CELL pCell = GetCellPtr(nCol, nRow);
    if(m_CurSelTile.nID == pCell->m_BottomID) return;
    __floodfill(nCol, nRow, pCell->m_BottomID, m_CurSelTile.nID, bVis, OUMAP_RES_TYPE::OUMRT_TILE);
    delete []bVis;    
    SetModified();
}

void OUMapView::FillCarpetTile(float fMX, float fMY)
{
    /** ���㵱ǰ�������λ�õĵ�ͼ���� */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    /** ��֤�Ϸ��� */
    if(nCol < 0 || nRow < 0) return;
    if(nCol >= m_dwWidth || nRow >= m_dwHeight) return;

    /** ��ʼFloodFill */
    bool* bVis = new bool[m_dwWidth * m_dwHeight];
    memset(bVis, 0, sizeof(bool) * m_dwWidth * m_dwHeight);
    POUMAP_CELL pCell = GetCellPtr(nCol, nRow);
    if(m_CurSelTile.nID == pCell->m_CarpetID) return;
    __floodfill(nCol, nRow, pCell->m_CarpetID, m_CurSelTile.nID, bVis, OUMAP_RES_TYPE::OUMRT_CARPET);
    delete []bVis;
    SetModified();
}

/** �����д���ͼ���ᱬջ�����д�˲��ܱ��� */
//#pragma comment(linker,"/STACK:102400000,1024000")

void OUMapView::__floodfill(int x, int y, int from, int to, bool vis[], OUMAP_RES_TYPE type)
{
    /** ��֤�Ϸ��� */
    if(x < 0 || y < 0) return;
    if(x >= m_dwWidth || y >= m_dwHeight) return;

    int idx = GetCellIdx(x, y);
    if(vis[idx]) return;

    if(type == OUMAP_RES_TYPE::OUMRT_TILE)
    {
        if(m_MapCell[idx].m_BottomID != from) return;

        /** ��� */
        vis[idx] = true;
        m_MapCell[idx].m_BottomID = to;
        m_MapCell[idx].m_pBottomImg = GetCellImage(to, OUMAP_RES_TYPE::OUMRT_TILE);
    }
    else
    if(type == OUMAP_RES_TYPE::OUMRT_CARPET)
    {
        if(m_MapCell[idx].m_CarpetID != from) return;

        /** ��� */
        vis[idx] = true;
        m_MapCell[idx].m_CarpetID = to;
        m_MapCell[idx].m_pCarpetImg = (to == OUMAP_TILE_BLACKID) ? NULL : (GetCellImage(to, OUMAP_RES_TYPE::OUMRT_CARPET));
    }

    /** 
     * ���ڽӵ����
     *
     *   �����ż���У����ڽӵ���(x - 1, y �� 1), (x, y �� 1)
     *   ����������У����ڽӵ���(x, y �� 1), (x + 1, y �� 1)
     *
     *                                          -- XadillaX
     */
    if(y & 1)
    {
        __floodfill(x + 1, y - 1, from, to, vis, type);
        __floodfill(x + 1, y + 1, from, to, vis, type);
        __floodfill(x, y - 1, from, to, vis, type);
        __floodfill(x, y + 1, from, to, vis, type);        
    }
    else
    {
        __floodfill(x - 1, y - 1, from, to, vis, type);
        __floodfill(x - 1, y + 1, from, to, vis, type);
        __floodfill(x, y - 1, from, to, vis, type);
        __floodfill(x, y + 1, from, to, vis, type);
    }
}

void OUMapView::MoveMapByMouse()
{
    if(!m_pHGE->Input_IsMouseOver()) return;

    static float lastmx = 0, lastmy = 0;
    static float lastmousedown = false;
    float mx, my;
    bool mousedown;
    m_pHGE->Input_GetMousePos(&mx, &my);
    mousedown = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);

    /** �����갴���� */
    if(mousedown && lastmousedown)
    {
        OUPOINT pos = GetTruePosOfScreen();

        pos.m_fX -= (mx - lastmx);
        pos.m_fY -= (my - lastmy);

        MoveArea(pos.m_fX, pos.m_fY);

        TRACE("λ��: (%d, %d)\n", (int)pos.m_fX, (int)pos.m_fY);
    }

    lastmx = mx, lastmy = my;
    lastmousedown = mousedown;
}

POINT OUMapView::GetCoorOfTileByTrueWorld(int xx, int yy)
{
    int nMouseWorldX = xx;
    int nMouseWorldY = yy;

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
    /** ƫ��ߵ����� */
    int nLeftRowCount = (m_dwHeight >> 1) + (m_dwHeight & 1);
    
    /** �ڵڼ���ƫ������� */
    int nLeftRow = nMouseWorldY / m_nTileHeight;
    
    /** ƫ�ϻ���ƫ�� */
    bool bUp = ((nMouseWorldY % m_nTileHeight) <= m_nHalfTileHeight);
    int RowMod = nMouseWorldY % m_nTileHeight;

    /** �ڵڼ����ݸ��� */
    int nColumn = nMouseWorldX / m_nTileWidth;

    /** ƫ����ƫ�� */
    bool bLeft = ((nMouseWorldX % m_nTileWidth) <= m_nHalfTileWidth);
    int ColMod = nMouseWorldX % m_nTileWidth;
 
    /** ƫ����ƫ�� */
    int nRow = 0;
    int nCol = 0;
    int x = ColMod;
    int y = RowMod;
    if(bUp && bLeft)
    {
        /** y = m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        /** y < m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        /** y > m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        if((m_nHalfTileHeight - y) * m_nHalfTileWidth > m_nHalfTileHeight * x)
        {
            /** ƫ�ұߵ���һ�� */
            nCol = nColumn - 1;
            nRow = (nLeftRow << 1) - 1;
        }
        else
        {
            /** ƫ��ߵ���һ�� */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
    }
    else
    /** ƫ����ƫ�� */
    if(bUp && !bLeft)
    {
        /** y = (m_nHalfTileHeight / m_nHalfTileWidth) * x - m_nHalfTileHeight */
        /** y < (m_nHalfTileHeight / m_nHalfTileWidth) * x - m_nHalfTileHeight */
        /** y > (m_nHalfTileHeight / m_nHalfTileWidth) * x - m_nHalfTileHeight */
        if((y + m_nHalfTileHeight) * m_nHalfTileWidth < m_nHalfTileHeight * x)
        {
            /** ƫ�ұߵ���һ�� */
            nCol = nColumn;
            nRow = (nLeftRow << 1) - 1;
        }
        else
        {
            /** ƫ��ߵ���һ�� */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
    }
    else
    /** ƫ����ƫ�� */
    if(!bUp && bLeft)
    {
        /** y = (m_nHalfTileHeight / m_nHalfTileWidth) * x + m_nHalfTileHeight */
        /** y < (m_nHalfTileHeight / m_nHalfTileWidth) * x + m_nHalfTileHeight */
        /** y > (m_nHalfTileHeight / m_nHalfTileWidth) * x + m_nHalfTileHeight */
        if((y - m_nHalfTileHeight) * m_nHalfTileWidth < m_nHalfTileHeight * x)
        {
            /** ƫ��ߵ���һ�� */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
        else
        {
            /** ƫ�ұߵ���һ�� */
            nCol = nColumn - 1;
            nRow = (nLeftRow << 1) + 1;
        }
    }
    else
    /** ƫ����ƫ�� */
    if(!bUp && !bLeft)
    {
        /** y = 3 * m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        /** y < 3 * m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        /** y > 3 * m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        if((3 * m_nHalfTileHeight - y) * m_nHalfTileWidth > m_nHalfTileHeight * x)
        {
            /** ƫ��ߵ���һ�� */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
        else
        {
            /** ƫ�ұߵ���һ�� */
            nCol = nColumn;
            nRow = (nLeftRow << 1) + 1;
        }
    }

    POINT pt;
    pt.x = nCol;
    pt.y = nRow;

    return pt;
}

POUMAP_UPPER_CELL OUMapView::GetFirstMapObjectByMouse(int xx, int yy)
{
    /** Խ������Խ�ϲ� */
    static hgeRect rect;
    for(list<POUMAP_UPPER_CELL>::reverse_iterator it = m_BuildingElement.rbegin(); it != m_BuildingElement.rend(); it++)
    {
        if(!(*it)->m_pImg) continue;
        OUPOINT sz = (*it)->m_pImg->GetSize();

        rect.Set((*it)->m_fTrueX, (*it)->m_fTrueY, (*it)->m_fTrueX + sz.m_fX, (*it)->m_fTrueY + sz.m_fY);
        if(rect.TestPoint(xx, yy))
        {
            /** ���ؼ���� */
            if(!(*it)->m_pImg->m_bStatic) return (*it); ///< ���������ؼ�����Ժ���˵
            else
            /** ���� */
            if((*it)->GetType() == OMOT_TABLE)
            {
                if(((OUMapTableImg*)((*it)->m_pImg))->TestPoint(xx - (int)rect.x1, yy - (int)rect.y1))
                {
                    return (*it);
                }
                else continue;
            }
            else
            {
                HTEXTURE hTex = (*it)->m_pImg->m_Img.m_pSprite->GetTexture();
                int w, h;
                w = m_pHGE->Texture_GetWidth(hTex);
                h = m_pHGE->Texture_GetHeight(hTex);
                DWORD* pBuff = m_pHGE->Texture_Lock(hTex);
                DWORD ok = pBuff[(xx - (int)rect.x1) + (yy - (int)rect.y1) * w];
                m_pHGE->Texture_Unlock(hTex);

                if(ok != 0) return (*it);
                else continue;
            }
            //return &(*it);
        }
    }

    return NULL;
}

void OUMapView::RenderState()
{
    if(!m_pInnerFont) return;

    static char info[256];
    //static OUFont* font = new OUFont("��Ȫ��ȿ�΢�׺�", 12, 0, 0, 1);

    //m_pInnerFont->SetKerningWidth(1);
    //m_pInnerFont->SetKerningHeight(2);

    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);

    static char layname[][7] = {
        "�ر��",
        "��̺��",
        "������"
    };

    sprintf(info,
        "֡������: %d FPS\n"              \
        "ʱ����: %.6f\n"                \
        "���λ��: (%.4d, %.4d)\n"        \
        "��Ⱦͼ��: %d\n\n"                \
        "ѡ��ͼ��: %s\n"                  \
        "ѡ��ͼ��: %.5d\n",
        (int)m_pHGE->Timer_GetFPS(),
        m_pHGE->Timer_GetDelta(),
        (int)mx, (int)my,
        m_dwRenderCount,
        layname[m_CurSelTile.emType],
        m_CurSelTile.nID);

    SIZE sz = m_pInnerFont->GetTextSize(C2W(info));
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    m_pInnerFont->SetColor(0xff000000);
    m_pInnerFont->Print(w - sz.cx - 10, 10, info);
    m_pInnerFont->SetColor(0xff6c9137);
    m_pInnerFont->Print(w - sz.cx - 11, 9, info);
}

void OUMapView::CalcTrueWorldSize()
{
    m_dwTrueHeight = m_nHalfTileHeight + (m_dwHeight * m_nHalfTileHeight);
    m_dwTrueWidth = m_nHalfTileWidth + (m_dwWidth * m_nTileWidth);
}

SIZE OUMapView::GetDocSize(int nWidth, int nHeight, int nTileWidth, int nTileHeight)
{
    SIZE sz;
    sz.cy = (nTileHeight >> 1) + (nHeight * (nTileHeight >> 1));
    sz.cx = (nTileWidth >> 1) + (nWidth * nTileWidth);

    return sz;
}

inline void __orderswap2(float& a, float& b)
{
    if(a < b) return;
    else
    {
        float c = a;
        a = b;
        b = c;
    }
}

bool OUMapView::TestRectInScreen(CRect& rect)
{
    return TestRectInScreen(rect.left, rect.top, rect.right, rect.bottom);
}

bool OUMapView::TestRectInScreen(float x1, float y1, float x2, float y2)
{
    /** ��С���Ū�� */
    __orderswap2(x1, x2);
    __orderswap2(y1, y2);

    float sx1, sy1, sx2, sy2;
    sx1 = m_nLeft;
    sy1 = m_nTop;
    sx2 = m_nLeft + m_nScreenWidth.GetValue();
    sy2 = m_nTop + m_nScreenWidth.GetValue();

    /** û�ཻ�������� */
    if(sx1 <= x1 && sy1 <= y1 && sx2 >= x2 && sy2 >= y2) return true;
    if(sx1 >= x1 && sy1 >= y1 && sx2 <= x2 && sy2 <= y2) return true;

    hgeRect rc(x1, y1, x2, y2);
    hgeRect screenrc(sx1, sy1, sx2, sy2);

    /** ���ཻ */
    if(rc.Intersect(&screenrc)) return true;
    
    /** ������ */
    return false;
}

void OUMapView::SetScreenSize(int nScreenWidth, int nScreenHeight)
{
    m_nScreenWidth = nScreenWidth;
    m_nScreenHeight = nScreenHeight;
}

void OUMapView::RecreateTarget(int nWidth, int nHeight)
{
    /** ɾ��ԭ���� */
    if(m_pMapSprite)
    {
        delete m_pMapSprite;
        m_pMapSprite = NULL;
    }

    /** �ͷ�ԭĿ�� */
    if(m_hMapTarget)
    {
        m_pHGE->Target_Free(m_hMapTarget);
        m_hMapTarget = 0;
    }

    /** ������Ŀ��;��� */
    m_hMapTarget = m_pHGE->Target_Create(nWidth, nHeight, true);
    m_pMapSprite = new hgeSprite(m_pHGE->Target_GetTexture(m_hMapTarget), 0, 0, nWidth, nHeight);
}

void OUMapView::RebuildTarget(float fDelta)
{
    if(m_hMapTarget == 0)
    {
        TRACE0("��ͼ����ȾĿ�ꡣ\n");
        return;
    }

    /**
     *
     * Whether to use texture interpolation or not.
     *
     *   Eg. it may be useful to temporarily disable
     *   texture filtering when rendering a tile map,
     *   as some videocard drivers have bugs resulting
     *   in incorrect tile junctions. This state may
     *   be set and changed at any time you want.  
     *
     *                                    --- HGE Document.
     *
     */
    m_pHGE->System_SetState(HGE_TEXTUREFILTER, false);

    m_pHGE->Gfx_BeginScene(m_hMapTarget);
    m_pHGE->Gfx_Clear(0xffcccccc);

    m_dwRenderCount = 0;
    int tx, ty;
    static bool updated[65536];
    static bool carupdated[65536];
    memset(updated, 0, sizeof(updated));
    memset(carupdated, 0, sizeof(carupdated));

    /** ��������Ҫ���� */
    if(m_CurSelTile.pImg && !m_CurSelTile.pImg->m_bStatic)
    {
        updated[m_CurSelTile.nID] = true;
        m_CurSelTile.pImg->GetAnimation()->Update(fDelta);
    }

    /** �����Ͻǿ�ʼ�� */
    for(int i = 0; i < m_dwHeight; i++)
    {
        /** ������ͼ��y��ʼλ�� */
        ty = i * m_nHalfTileHeight;
        
        /** ����ʼλ�ò�������� */
        if(ty + m_nTileHeight <= m_nTop) continue;
        
        /** ����ʼλ�ó��������� */
        if(ty >= m_nTop + GetScreenHeight()) break;
        
        /** ö�ٱ��и�����ͼ */
        for(int j = 0; j < m_dwWidth; j++)
        {
            /** ����ͼxλ�� */
            tx = j * m_nTileWidth;
            if(i % 2 == 1) tx += m_nHalfTileWidth;
            
            /** ����ʼx��������� */
            if(tx + m_nTileWidth <= m_nLeft) continue;
            
            /** ����ʼλ�ó��������� */
            if(tx >= m_nLeft + GetScreenWidth()) break;
            
            /** ��ȡ��ͼ��Ϣ */
            OUMAP_CELL cell = GetCell(j, i);
            
            if(cell.m_pBottomImg != NULL)
            {
                /** ���Ƕ�̬��ͼ����¶��� */
                if(!cell.m_pBottomImg->m_bStatic && !updated[cell.m_BottomID])
                {
                    updated[cell.m_BottomID] = true;
                    cell.m_pBottomImg->GetAnimation()->Update(fDelta);
                }
                
                /** ��Ⱦ��ͼ��Ŀ�� */
                cell.m_pBottomImg->RenderStretch(tx - m_nLeft, ty - m_nTop, m_nTileWidth, m_nTileHeight);

                /** ��Ⱦͼ���һ */
                m_dwRenderCount++;
            }

            if(cell.m_pCarpetImg != NULL)
            {
                /** ���Ƕ�̬��ͼ����¶��� */
                if(!cell.m_pCarpetImg->m_bStatic && !carupdated[cell.m_CarpetID])
                {
                    carupdated[cell.m_CarpetID] = true;
                    cell.m_pCarpetImg->GetAnimation()->Update(fDelta);
                }

                /** ��Ⱦ��ͼ��Ŀ�� */
                cell.m_pCarpetImg->RenderStretch(tx - m_nLeft, ty - m_nTop, m_nTileWidth, m_nTileHeight);

                /** ��Ⱦͼ���һ */
                m_dwRenderCount++;
            }

            if(m_bShowGrid.GetValue() && !m_bUpperGrid.GetValue())
            {
                int ltx, lty;
                ltx = tx - m_nLeft;
                lty = ty - m_nTop;
                
                DrawSingleGrid(ltx, lty, 0xff00ff00);
            }
        }
    }

    /** ���� */
    OUPOINT sz;
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        /** ��� */
        if(!(*it)->m_pImg) continue;
        OUPOINT sz = (*it)->m_pImg->GetSize();

        /** �������� */
        if(!TestRectInScreen((*it)->m_fTrueX, (*it)->m_fTrueY, (*it)->m_fTrueX + sz.m_fX, (*it)->m_fTrueY + sz.m_fY))
        {
            continue;
        }

        /** ����ǽ���ͼ�� */
        if((*it) == OUMAP_UPPER_CELL::m_pFocused)
        {
            (*it)->m_pImg->SetColor(0xffff7800);
            if((*it)->GetType() == OUMAP_OBJECT_TYPE::OMOT_BUILDING)
            {
                (*it)->m_pImg->Render((*it)->m_fTrueX - m_nLeft, (*it)->m_fTrueY - m_nTop);
            }
            else
            {
                OUMapTable* p = (OUMapTable*)(*it);
                ((OUMapTableImg*)(p->m_pImg))->RenderWith(p->m_fTrueX - m_nLeft, p->m_fTrueY - m_nTop, p->GetTableID());
            }
            (*it)->m_pImg->SetColor(0xffffffff);
        }
        else
        /** ����ǽ��㱻ѡ��ͼ�� */
        if((*it) == OUMAP_UPPER_CELL::m_pClickFocused)
        {
            (*it)->m_pImg->SetColor(0xffff0099);
            if((*it)->GetType() == OUMAP_OBJECT_TYPE::OMOT_BUILDING)
            {
                (*it)->m_pImg->Render((*it)->m_fTrueX - m_nLeft, (*it)->m_fTrueY - m_nTop);
            }
            else
            {
                OUMapTable* p = (OUMapTable*)(*it);
                ((OUMapTableImg*)(p->m_pImg))->RenderWith(p->m_fTrueX - m_nLeft, p->m_fTrueY - m_nTop, p->GetTableID());
            }
            (*it)->m_pImg->SetColor(0xffffffff);
        }
        else
        {
            if((*it)->GetType() == OUMAP_OBJECT_TYPE::OMOT_BUILDING)
            {
                (*it)->m_pImg->Render((*it)->m_fTrueX - m_nLeft, (*it)->m_fTrueY - m_nTop);
            }
            else
            {
                OUMapTable* p = (OUMapTable*)(*it);
                ((OUMapTableImg*)(p->m_pImg))->RenderWith(p->m_fTrueX - m_nLeft, p->m_fTrueY - m_nTop, p->GetTableID());
            }
        }

        /** ��Ⱦͼ���һ */
        m_dwRenderCount++;
    }

    /** �ϲ����� */
    if(m_bUpperGrid.GetValue()) DrawUpperGrid();

    m_pHGE->Gfx_EndScene();

    m_pHGE->System_SetState(HGE_TEXTUREFILTER, true);
}

void OUMapView::DrawUpperGrid()
{
    POUGUI_IMG pGroundImg = GetCellImage(OUMAP_TILE_GROUND, OUMRT_TILE);
    POUGUI_IMG pFocusImg = GetCellImage(OUMAP_TILE_FOCUS, OUMRT_TILE);
    POUGUI_IMG pBlockImg = GetCellImage(OUMAP_TILE_BLOCK, OUMRT_TILE);

    /** ������� */
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);
    POINT mscoor = GetCoorOfTileByTrueWorld(mx + m_nLeft, my + m_nTop);

    int tx, ty;
    POUMAP_CELL pCell;
    for(int i = 0; i < m_dwHeight; i++)
    {
        ty = i * m_nHalfTileHeight;
        if(ty + m_nTileHeight <= m_nTop) continue;
        if(ty >= m_nTop + GetScreenHeight()) break;

        for(int j = 0; j < m_dwWidth; j++)
        {
            tx = j * m_nTileWidth;
            if(i % 2 == 1) tx += m_nHalfTileWidth;
            if(tx + m_nTileWidth <= m_nLeft) continue;
            if(tx >= m_nLeft + GetScreenWidth()) break;

            DrawSingleGrid(tx - m_nLeft, ty - m_nTop, 0xffff0000);

            /** ������ϰ���ģʽ */
            OUMAP_TILE_COOR coor = { j, i };
            if(m_emEditMode == OUMEM_BLOCK)
            {
                /** ������ϰ��� */
                pCell = GetCellPtr(j, i);
                if(NULL != pCell && pCell->block && pBlockImg)
                {
                    pBlockImg->Render(tx - m_nLeft, ty - m_nTop);
                }
                else
                /** ������������������ */
                if(mscoor.x == coor.x && mscoor.y == coor.y)
                {
                    if(m_pHGE->Input_IsMouseOver())
                    {
                        if(pFocusImg)
                        {
                            pFocusImg->Render(tx - m_nLeft, ty - m_nTop);
                        }
                    }
                }
            }
            else
            /** ����ǵػ�ģʽ */
            if(m_emEditMode == OUMEM_GROUND)
            {
                /** �����ѡ�е� */
                if(OUMAP_UPPER_CELL::m_pClickFocused)
                {
                    /** �����������ǵػ� */
                    OUMAP_TILE_COOR coor = { j, i };
                    if(OUMAP_UPPER_CELL::m_pClickFocused->m_pGroundSet->find(coor) != OUMAP_UPPER_CELL::m_pClickFocused->m_pGroundSet->end())
                    {
                        if(pGroundImg)
                        {
                            pGroundImg->Render(tx - m_nLeft, ty - m_nTop);
                        }
                    }
                    /** ������������������ */
                    else
                    if(mscoor.x == coor.x && mscoor.y == coor.y)
                    {
                        if(m_pHGE->Input_IsMouseOver())
                        {
                            if(pFocusImg)
                            {
                                pFocusImg->Render(tx - m_nLeft, ty - m_nTop);
                            }
                        }
                    }
                }
            }
        }
    }
}

void OUMapView::DrawSingleGrid(int sx, int sy, DWORD color)
{
    int ltx = sx, lty = sy;
    m_pHGE->Gfx_RenderLine(ltx + m_nHalfTileWidth, lty,
        ltx + m_nTileWidth, lty + m_nHalfTileHeight, color);
    m_pHGE->Gfx_RenderLine(ltx + m_nTileWidth, lty + m_nHalfTileHeight,
        ltx + m_nHalfTileWidth, lty + m_nTileHeight, color);
    m_pHGE->Gfx_RenderLine(ltx + m_nHalfTileWidth, lty + m_nTileHeight, 
        ltx, lty + m_nHalfTileHeight, color);
    m_pHGE->Gfx_RenderLine(ltx, lty + m_nHalfTileHeight, 
        ltx + m_nHalfTileWidth, lty, color);
}

void OUMapView::ClearTarget()
{
    /** ɾ��ԭ���� */
    if(m_pMapSprite)
    {
        delete m_pMapSprite;
        m_pMapSprite = NULL;
    }

    /** �ͷ�ԭĿ�� */
    if(m_hMapTarget)
    {
        m_pHGE->Target_Free(m_hMapTarget);
    }
}

POUGUI_IMG OUMapView::GetCellImage(int id, OUMAP_RES_TYPE type)
{
    switch(type)
    {
        /** �صײ� */
    case OUMRT_TILE:
        {
            if(m_Tiles.find(id) == m_Tiles.end())
            {
                /** ��û�ҵ��������� */
                LoadImage(id, type);
            }

            /** ����û�ҵ������Ҳ����� */
            if(m_Tiles.find(id) == m_Tiles.end()) return NULL;
            
            return &m_Tiles[id].img;

            break;
        }

        /** ��̺�� */
    case OUMRT_CARPET:
        {
            if(m_CarpetRes.find(id) == m_CarpetRes.end())
            {
                LoadImage(id, type);
            }

            if(m_CarpetRes.find(id) == m_CarpetRes.end()) return NULL;

            return &m_CarpetRes[id].img;

            break;
        }

        /** ������ */
    case OUMRT_BUILDING:
        {
            if(m_BuildingRes.find(id) == m_BuildingRes.end())
            {
                LoadImage(id, type);
            }

            if(m_BuildingRes.find(id) == m_BuildingRes.end()) return NULL;

            return &m_BuildingRes[id].img;

            break;
        }

        /** ���Ӳ� */
    case OUMRT_TABLE:
        {
            if(m_Table.find(id) == m_Table.end())
            {
                LoadImage(id, type);
            }

            if(m_Table.find(id) == m_Table.end()) return NULL;

            return m_Table[id].pImg;

            break;
        }

    default:
        return NULL;
    }
}

DWORD transColor = 0xffff00ff;

bool OUMapView::LoadImage(int id, OUMAP_RES_TYPE type)
{
    map<int, OUMAP_TEX_REF>* pContainer = NULL;
    char prefix[256] = { 0 };
    switch(type)
    {
        /** ��ͼ */
    case OUMRT_TILE:
        {
            pContainer = &m_Tiles;
            sprintf(prefix, OUMAP_TILE_NAME_FORMAT, id);

            /** �ػ���ͼ */
            if(id == OUMAP_TILE_FOCUS)
            {
                sprintf(prefix, "tile\\ground\\focus");
            }
            else
            if(id == OUMAP_TILE_GROUND)
            {
                sprintf(prefix, "tile\\ground\\ground");
            }
            else
            if(id == OUMAP_TILE_BLOCK)
            {
                sprintf(prefix, "tile\\ground\\block");
            }

            break;
        }

        /** ��̺ */
    case OUMRT_CARPET:
        {
            pContainer = &m_CarpetRes;
            sprintf(prefix, OUMAP_CARPET_NAME_FORMAT, id);
            break;
        }

        /** ���� */
    case OUMRT_BUILDING:
        {
            pContainer = &m_BuildingRes;
            sprintf(prefix, OUMAP_BUILDING_NAME_FORMAT, id);
            break;
        }

    case OUMRT_TABLE:
        {
            OUMAP_TEX_REF ref;
            ref.pImg = new OUMapTableImg();
            sprintf(prefix, "tile\\building\\table%d.ini", id);
            ref.pImg->LoadFromIni(prefix);

            m_Table[id] = ref;

            return true;
        }

    default:
        break;
    }

    if(!pContainer || prefix[0] == 0) return false;

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
            
            /** ������ʧ������� */
            ref.img.m_Img.m_pAnim = new OUAniFile();
            if(!ref.img.m_Img.m_pAnim->LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize))
            {
                continue;
            }

            ref.width = ref.img.m_Img.m_pAnim->GetAnimation()->GetWidth();
            ref.height = ref.img.m_Img.m_pAnim->GetAnimation()->GetHeight();
            ref.img.Play();
            
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

            if(ref.tex == 0) continue;

            /** ��ʵ��� */
            int iw, ih;
            iw = m_pHGE->Texture_GetWidth(ref.tex, true);
            ih = m_pHGE->Texture_GetHeight(ref.tex, true);

            /** 2�Ĵ��ݿ�ߡ�����������Դ 2012/12/5 23:57 */
            int fw, fh;
            fw = m_pHGE->Texture_GetWidth(ref.tex);
            fh = m_pHGE->Texture_GetHeight(ref.tex);

            /** ͸��ɫ */
            if(transColor != 0)
            {
                DWORD* mem = m_pHGE->Texture_Lock(ref.tex, false);
                int imax = fw * fh;
                for(int j = 0; j < imax; j++)
                {
                    if(mem[j] == transColor) mem[j] = 0;
                }
                m_pHGE->Texture_Unlock(ref.tex);
            }

            ref.img.m_Img.m_pSprite = new hgeSprite(ref.tex, 0, 0, iw, ih);
            ref.width = iw;
            ref.height = ih;

            (*pContainer)[id] = ref;

            return true;
        }
    }

    return false;
}

bool OUMapView::MoveArea(int dwTrueX, int dwTrueY, bool bScrollBy)
{
    int left, top;
    left = m_nLeft;
    top = m_nTop;

    int dwScreenWidth = m_nScreenWidth.GetValue();
    int dwScreenHeight = m_nScreenHeight.GetValue();

    /** ������ʾ��ߺ��ұߵļ� --- ���Ǳ༭��������ʾ */
    if(m_dwTrueWidth - m_nTileWidth < dwScreenWidth)
    {
        m_nLeft = -(dwScreenWidth - m_dwTrueWidth) / 2;
    }
    else
    {
        //if(dwTrueX < m_nHalfTileWidth) dwTrueX = m_nHalfTileWidth;
        //if(dwTrueX + dwScreenWidth + m_nHalfTileWidth > m_dwTrueWidth) dwTrueX = m_dwTrueWidth - dwScreenWidth - m_nHalfTileWidth;

        if(dwTrueX < 0) dwTrueX = 0;
        if(dwTrueX + dwScreenWidth > m_dwTrueWidth) dwTrueX = m_dwTrueWidth - dwScreenWidth;

        m_nLeft = dwTrueX;
    }

    /** ������ʾ���µļ� --- ���Ǳ༭��������ʾ */
    if(m_dwTrueHeight - m_nTileHeight < dwScreenHeight)
    {
        m_nTop = -(dwScreenHeight - m_dwTrueHeight) / 2;
    }
    else
    {
        //if(dwTrueY < m_nHalfTileHeight) dwTrueY = m_nHalfTileHeight;
        //if(dwTrueY + dwScreenHeight + m_nHalfTileHeight > m_dwTrueHeight) dwTrueY = m_dwTrueHeight - dwScreenHeight - m_nHalfTileHeight;
        
        if(dwTrueY < 0) dwTrueY = 0;
        if(dwTrueY + dwScreenHeight > m_dwTrueHeight) dwTrueY = m_dwTrueHeight - dwScreenHeight;

        m_nTop = dwTrueY;
    }

    /** �ع���ͼ��ͼ */
    //RebuildTile(fDelta);

    if(m_nLeft == left && m_nTop == top) return false;
    else
    {
        /** ��������������ƶ�������������� */
        if(!bScrollBy)
        {
            POINT pt;
            pt.x = m_nLeft;
            pt.y = m_nTop;
            ::g_pViewDlg->ScrollToPosition(pt);
        }
        return true;
    }
}

void OUMapView::SendMessage(OUMAPVIEWMSG Msg, LPVOID lpParam, bool bAutoDel)
{
    OUMAP_MSG_QUEUE_ITEM item;
    item.emMsg = Msg;
    item.lpParam = lpParam;
    item.bAutoDel = bAutoDel;

    ::EnterCriticalSection(&m_MsgQueueCS);
    m_MsgQueue.push(item);
    ::LeaveCriticalSection(&m_MsgQueueCS);
}

void OUMapView::TranslateMessage(OUMAP_MSG_QUEUE_ITEM Msg)
{
    switch(Msg.emMsg)
    {
        /** �����С�ı� */
    case OM_SIZE:
        {
            OM_SIZE_PARAM* param = (OM_SIZE_PARAM*)Msg.lpParam;
            int cx = param->m_nScreenWidth;
            int cy = param->m_nScreenHeight;

            m_pHGE->System_SetState(HGE_SCREENWIDTH, cx);
            m_pHGE->System_SetState(HGE_SCREENHEIGHT, cy);

            ::MoveWindow(m_pHGE->System_GetState(HGE_HWND),0, 0, cx, cy, true);

            SetScreenSize(cx, cy);
            RecreateTarget(cx, cy);

            break;
        }

        /** �½���ͼ��Ϣ */
    case OM_NEWMAP:
        {
            OM_NEWMAP_PARAM* param = (OM_NEWMAP_PARAM*)Msg.lpParam;
            NewMap(param->m_szName, param->m_nWidth, param->m_nHeight, param->m_nTileWidth, param->m_nTileHeight);
            break;
        }

        /** �޸ĵ�ǰ���� */
    case OM_SELECTTILE:
        {
            OM_SELECTTILE_PARAM* param = (OM_SELECTTILE_PARAM*)Msg.lpParam;

            /** �صײ� */
            if(param->m_emResType == OUMRT_TILE)
            {
                m_CurSelTile.emType = (OUMAP_RES_TYPE)param->m_emResType;
                m_CurSelTile.nID = param->m_nID;

                m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
            }
            else
            /** ��̺�� */
            if(param->m_emResType == OUMRT_CARPET)
            {
                m_CurSelTile.emType = (OUMAP_RES_TYPE)param->m_emResType;
                m_CurSelTile.nID = param->m_nID;

                m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
            }
            else
            /** ������ */
            if(param->m_emResType == OUMRT_BUILDING)
            {
                if(param->m_nID < 1e9)
                {
                    m_CurSelTile.emType = OUMRT_BUILDING;
                    m_CurSelTile.nID = param->m_nID;

                    m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
                }
                else
                {
                    m_CurSelTile.emType = OUMRT_TABLE;
                    m_CurSelTile.nID = 2147483647 - param->m_nID + 1;

                    m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
                }
            }
            else
            {
                /** TODO */
            }

            break;
        }

        /** �޸ı༭ģʽ���� */
    case OM_CHANGEEDITMODE:
        {
            OM_CHANGEEDITMODE_PARAM* param = (OM_CHANGEEDITMODE_PARAM*)Msg.lpParam;

            /** ���ԭ������ѡ�������ڲ������������ */
            if(m_emEditMode == OUMEM_CHOOSE || m_emEditMode == OUMEM_ERASE || m_emEditMode == OUMEM_GROUND)
            {
                if(param->m_emEditMode != OUMEM_CHOOSE && param->m_emEditMode != OUMEM_ERASE && param->m_emEditMode != OUMEM_GROUND)
                {
                    OUMAP_UPPER_CELL::m_pFocused = NULL;
                    OUMAP_UPPER_CELL::m_pClickFocused = NULL;
                }
                else
                if(param->m_emEditMode == OUMEM_GROUND)
                {
                    OUMAP_UPPER_CELL::m_pFocused = NULL;
                }
            }

            m_emEditMode = (OUMAP_EDIT_MODE)param->m_emEditMode;
            break;
        }

        /** �������ƶ���ͼ */
    case OM_SCROLLMOVEAREA:
        {
            OM_SCROLLMOVEAREA_PARAM* param = (OM_SCROLLMOVEAREA_PARAM*)Msg.lpParam;

            MoveArea(param->m_nToX, param->m_nToY, true);
            break;
        }

        /** �ı��ڲ����� */
    case OM_SETINNERFONT:
        {
            OM_SETINNERFONT_PARAM* param = (OM_SETINNERFONT_PARAM*)Msg.lpParam;

            SAFEDEL(m_pInnerFont);
            m_pInnerFont = new OUFont(param->m_szFont, param->m_nSize, param->m_bBold, param->m_bItalic, param->m_bAntialias);

            m_pInnerFont->SetKerningWidth(1);
            m_pInnerFont->SetKerningHeight(2);

            break;
        }

    default:
        break;
    }

    if(Msg.bAutoDel)
    {
        delete Msg.lpParam;
    }
}
