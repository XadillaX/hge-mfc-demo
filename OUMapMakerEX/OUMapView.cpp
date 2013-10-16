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

    /** 动画环境 */
    ::OUAniFile::InitAll();
    ::OUMapTableImg::InitAll();

    /** 载入黑图 */
    //OUMAP_TEX_REF black;
    //black.id = 0;
    //black.tex = m_pHGE->Texture_Load("tile\\blacktile.png");
    //if(black.tex != 0)
    //{
    //    black.img.m_Img.m_pSprite = new hgeSprite(black.tex, 0, 0, m_nTileWidth, m_nTileHeight);
    //}
    //m_Tiles[OUMAP_TILE_BLACKID] = black;

    SetScreenSize(m_pHGE->System_GetState(HGE_SCREENWIDTH), m_pHGE->System_GetState(HGE_SCREENHEIGHT));
    
    /** 目标大小 */
    OM_SIZE_PARAM* param = new OM_SIZE_PARAM();
    param->m_nScreenWidth = m_pHGE->System_GetState(HGE_SCREENWIDTH);
    param->m_nScreenHeight = m_pHGE->System_GetState(HGE_SCREENHEIGHT);
    SendMessage(OM_SIZE, param);

    /** 新建地图 */
    OM_NEWMAP_PARAM* param2 = new OM_NEWMAP_PARAM;
    strcpy(param2->m_szName, "无标题");
    param2->m_nWidth = DEFAULT_MAP_WIDTH;
    param2->m_nHeight = DEFAULT_MAP_HEIGHT;
    param2->m_nTileWidth = DEFAULT_TILE_WIDTH;
    param2->m_nTileHeight = DEFAULT_TILE_HEIGHT;
    SendMessage(OM_NEWMAP, param2);

    /** 当前选择图块 */
    m_CurSelTile.nID = 0;
    m_CurSelTile.emType = OUMRT_TILE;
    m_CurSelTile.pImg = NULL;

    /** 鼠标指针 */
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

    /** 内部字体 */
    OM_SETINNERFONT_PARAM* ftParam = new OM_SETINNERFONT_PARAM();
    strcpy(ftParam->m_szFont, g_IniFile.GetString("dxfont", "fontname", "宋体").c_str());
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

    /** 鼠标指针 */
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

    /** 动画环境 */
    ::OUMapTableImg::ReleaseAll();
    ::OUAniFile::ReleaseAll();
}

void OUMapView::NewMap(const char* name, int w, int h, int tw, int th)
{
    /** 关闭原先地图 */
    CloseMap();

    /** 贴图宽高 */
    m_nTileWidth = tw;
    m_nTileHeight = th;
    m_nHalfTileWidth = tw >> 1;
    m_nHalfTileHeight = th >> 1;

    ///** 底层贴图基础路径 */
    //static char basepath[256];
    //sprintf(basepath, "%dx%d", tw, th);
    //m_szBasePath = basepath;

    /** 载入黑图 */
    GetCellImage(OUMAP_TILE_BLACKID, OUMRT_TILE);
    GetCellImage(OUMAP_TILE_FOCUS, OUMRT_TILE);

    /** 宽高 */
    m_dwWidth = w;
    m_dwHeight = h;
    CalcTrueWorldSize();

    /** 左上角位置 */
    m_nLeft = 0;
    m_nTop = 0;

    /** 地图名 */
    m_szName = name;

    /** 地图Cell */
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

    /** 当前选择 */
    if(m_CurSelTile.pImg != NULL)
    {
        m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
    }
}

void OUMapView::CloseMap()
{
    /** 地图 */
    m_MapCell.clear();
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        delete *it;
    }
    m_BuildingElement.clear();
    //m_CarpetElement.clear();

    /** 清除目标 */
    //ClearTarget();

    /** 地底层资源 */
    for(map<int, OUMAP_TEX_REF>::iterator it = m_Tiles.begin(); it != m_Tiles.end(); it++)
    {
        it->second.img.Release();
        m_pHGE->Texture_Free(it->second.tex);
    }
    m_Tiles.clear();

    /** 地毯层资源 */
    for(map<int, OUMAP_TEX_REF>::iterator it = m_CarpetRes.begin(); it != m_CarpetRes.end(); it++)
    {
        it->second.img.Release();
        m_pHGE->Texture_Free(it->second.tex);
    }
    m_CarpetRes.clear();

    /** 建筑层资源 */
    for(map<int, OUMAP_TEX_REF>::iterator it = m_BuildingRes.begin(); it != m_BuildingRes.end(); it++)
    {
        it->second.img.Release();
        m_pHGE->Texture_Free(it->second.tex);
    }
    m_BuildingRes.clear();

    /** 清除宽高 */
    m_dwWidth = m_dwHeight = 0;
    CalcTrueWorldSize();

    /** 清除名字 */
    m_szName = "";

    /** 左上角位置 */
    m_nLeft = m_nTop = 0;

    /** 当前选择 */
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

    /** 文件头 */
    OUMAP_MAPFILE_HEADER header;
    fread(&header, sizeof(header), 1, fp);
    if(header.m_dwMagicNum != OUMAP_MAGIC_NUMBER) return FALSE;
    if(header.m_dwHeaderSize != sizeof(OUMAP_MAPFILE_HEADER)) return FALSE;

    /** 宽高 */
    m_dwWidth = header.m_dwMapWidth;
    m_dwHeight = header.m_dwMapHeight;
    m_nTileWidth = header.m_dwTileWidth;
    m_nTileHeight = header.m_dwTileHeight;
    m_nHalfTileWidth = header.m_dwTileWidth >> 1;
    m_nHalfTileHeight = header.m_dwTileHeight >> 1;
    m_szName = header.m_szMapName;

    this->CalcTrueWorldSize();
    
    /** 每个图块 */
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

    /** 所有物件信息 */
    OUMAP_UPPER_CELL_TOSAVE utoread;
    POUMAP_UPPER_CELL pbuildingcell;
    vector<int> cnt;
    int tablecnt = 0;
    for(int i = 0; i < header.m_dwObjectCount; i++)
    {
         fread(&utoread, sizeof(OUMAP_UPPER_CELL_TOSAVE), 1, fp);

         /** 建筑 */
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

    /** 遮罩层 */
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

    /** 桌子层 */    
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

    /** 当前选择 */
    if(m_CurSelTile.pImg != NULL)
    {
        m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
    }

    return TRUE;
}

BOOL OUMapView::SaveMap(LPCTSTR szFilename)
{
    /** 地图结构 */
    /**
     *
     * Header -> 每个Tile的结构 -> 每个物件的信息 -> 遮罩信息
     *
     */
    FILE* fp = fopen(szFilename, "wb+");
    if(fp == NULL) return FALSE;
    
    /** 文件头 */
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

    /** 每格信息 */
    int cellcount = m_dwWidth * m_dwHeight;
    OUMAP_CELL_TOSAVE tosave;
    for(int i = 0; i < cellcount; i++)
    {
        tosave.m_bBlock = m_MapCell[i].block;
        tosave.m_dwBottomID = m_MapCell[i].m_BottomID;
        tosave.m_dwCarpetID = m_MapCell[i].m_CarpetID;

        fwrite(&tosave, sizeof(tosave), 1, fp);
    }

    /** 物件信息 */
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

    /** 遮罩信息 */
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

    /** 桌子信息 */
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
        TRACE0("地图精灵指针为空。\n");
    }

    /** 画鼠标 */
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);
    if(m_pHGE->Input_IsMouseOver() && (m_emEditMode == OUMEM_ADD || m_emEditMode == OUMEM_FILL))
    {
        /** 有图片 */
        if(m_CurSelTile.pImg)
        {
            switch(m_CurSelTile.emType)
            {
                /** 选中地表贴图 */
            case OUMRT_TILE:
                {
                    if(OUMAP_TILE_BLACKID == m_CurSelTile.nID) break;

                    /** 画个重影 */
                    if(m_Tiles.find(OUMAP_TILE_BLACKID) != m_Tiles.end())
                    {
                        m_Tiles[OUMAP_TILE_BLACKID].img.RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight + 10, m_nTileWidth, m_nTileHeight);
                    }

                    m_CurSelTile.pImg->RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight, m_nTileWidth, m_nTileHeight);

                    /** 画描边 */
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

                /** 选中地毯贴图 */
            case OUMRT_CARPET:
                {
                    /** Fill状态不能填充——所以是xx */
                    //if(m_emEditMode == OUMEM_FILL)
                    //{
                    //    m_pCantCursor->SetColor(m_pHGE->Input_GetKeyState(HGEK_LBUTTON) ? 0xffff0000 : 0xffffffff);
                    //    m_pCantCursor->Render(mx, my);
                    //    break;
                    //}

                    if(OUMAP_TILE_BLACKID == m_CurSelTile.nID) break;

                    /** 画个重影 */
                    //if(m_Tiles.find(OUMAP_TILE_BLACKID) != m_Tiles.end())
                    //{
                    //    m_Tiles[OUMAP_TILE_BLACKID].img.RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight + 10, m_nTileWidth, m_nTileHeight);
                    //}

                    m_CurSelTile.pImg->RenderStretch(mx - m_nHalfTileWidth, my - m_nHalfTileHeight, m_nTileWidth, m_nTileHeight);

                    break;
                }

                /** 选中建筑贴图 */
            case OUMRT_BUILDING:
                {
                    /** Fill状态不能填充——所以是xx */
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

                /** 桌子贴图 */
            case OUMRT_TABLE:
                {
                    /** Fill状态不能填充——所以是xx */
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

    /** 画鼠标指针 */
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
    /** 消息循环 */
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

    /** 上下左右移动键盘 */
    for(int i = HGEK_LEFT; i <= HGEK_DOWN; i++)
    {
        if(m_pHGE->Input_GetKeyState(i))
        {
            pos.m_fX += (dirx[i - HGEK_LEFT] * 20);
            pos.m_fY += (diry[i - HGEK_LEFT] * 20);
        }   
    }

    /** 鼠标滚轮 */
    int nWheel = m_pHGE->Input_GetMouseWheel();
    if(nWheel) pos.m_fY -= (nWheel * 50);
    
    MoveArea(pos.m_fX, pos.m_fY);

    /** 编辑模式 */
    switch(m_emEditMode)
    {
        /** 选择模式 */
    case OUMEM_CHOOSE:
        {
            ChooseMapObject();
            break;
        }

        /** 地基模式 */
    case OUMEM_GROUND:
        {
            DoGround();
            break;
        }

        /** 移动模式 */
    case OUMEM_MOVE: MoveMapByMouse(); break;

        /** 障碍物模式 */
    case OUMEM_BLOCK:
        {
            DoBlock();
            break;
        }

        /** 橡皮擦模式 */
    case OUMEM_ERASE:
        {
            /** 橡皮擦的时候也要选中物件——只不过不break */
            ChooseMapObject();
        }

    default: DoEditMap(); break;
    }

    /** 删除键 */
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

    /** 没有选中的物件 */
    if(OUMAP_UPPER_CELL::m_pClickFocused == NULL) return;

    POUMAP_UPPER_CELL pCell = OUMAP_UPPER_CELL::m_pClickFocused;
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);

    /** 计算当前鼠标所在位置的地图坐标 */
    int nMouseWorldX = m_nLeft + mx;
    int nMouseWorldY = m_nTop + my;

    /** 贴图坐标 */
    POINT coor = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    OUMAP_TILE_COOR MapCoor;
    MapCoor.x = coor.x;
    MapCoor.y = coor.y;

    if(m_pHGE->Input_GetKeyState(HGEK_LBUTTON))
    {
        /** 加 */
        pCell->m_pGroundSet->insert(MapCoor);

        /** 设置修改 */
        SetModified();
    }
    else
    if(m_pHGE->Input_GetKeyState(HGEK_RBUTTON))
    {
        /** 减 */
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

    /** 计算当前鼠标所在位置的地图坐标 */
    int nMouseWorldX = m_nLeft + mx;
    int nMouseWorldY = m_nTop + my;

    /** 贴图坐标 */
    POINT coor = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    if(coor.x < 0 || coor.y < 0 || coor.x >= m_dwWidth || coor.y >= m_dwHeight) return;

    POUMAP_CELL pCell = GetCellPtr(coor.x, coor.y);

    if(m_pHGE->Input_GetKeyState(HGEK_LBUTTON))
    {
        /** 加 */
        pCell->block = true;
        SetModified();
    }
    else
    if(m_pHGE->Input_GetKeyState(HGEK_RBUTTON))
    {
        /** 减 */
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

    /** 计算当前鼠标所在位置的地图坐标 */
    int nMouseWorldX = m_nLeft + mx;
    int nMouseWorldY = m_nTop + my;

    /** 没按下——则获取焦点对象 */
    if(!bLButtonDown)
    {
        OUMAP_UPPER_CELL::m_pFocused = GetFirstMapObjectByMouse(nMouseWorldX, nMouseWorldY);
    }
    else
    /** 如果按下则更新层或者移动 */
    if(bLButtonDown)
    {
        OUMAP_UPPER_CELL::m_pClickFocused = OUMAP_UPPER_CELL::m_pFocused;

        /** 如果是第一次按下，则将该物件移至顶层 */
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
        /** 否则移动 */
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
        /** 绘画模式 */
    case OUMEM_ADD:
        {
            AddTile(mx, my);
            break;
        }

        /** 填充模式 */
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

                /** TODO: 其它层 */

                /** 其余层不能用填充 */
            default: break;
            }

            break;
        }

        /** 橡皮模式 */
    case OUMEM_ERASE:
        {
            switch(m_CurSelTile.emType)
            {
            case OUMAP_RES_TYPE::OUMRT_CARPET:
            case OUMAP_RES_TYPE::OUMRT_TILE:
                {
                    if(!bLButtonDown) break;

                    /** 如果是地毯层和地底层 */
                    EraseTile(mx, my);
                    break;
                }

            case OUMAP_RES_TYPE::OUMRT_TABLE:
            case OUMAP_RES_TYPE::OUMRT_BUILDING:
                {
                    /** 如果是建筑层 */
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

    /** 当前选择 */
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
            /** 在建筑层鼠标弹起才算贴图 */
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

        /** TODO: 其它层 */
    }
}

void OUMapView::AddBottomTile(float fMX, float fMY)
{
    /** 计算当前鼠标所在位置的地图坐标 */
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
    /** 计算当前鼠标所在位置的地图坐标 */
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

    /** 计算当前鼠标所在位置的地图坐标 */
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

    /** 计算当前鼠标所在位置的地图坐标 */
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
    /** 当前选择 */
    switch(m_CurSelTile.emType)
    {
    case OUMRT_TILE:
        {
            //if(m_CurSelTile.nID == OUMAP_TILE_BLACKID) break;
            EraseBottomTile(fMX, fMY);
            break;
        }

        /** TODO: 其它层 */
    }
}

void OUMapView::EraseBottomTile(float fMX, float fMY)
{
    /** 计算当前鼠标所在位置的地图坐标 */
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
    /** 计算当前鼠标所在位置的地图坐标 */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    /** 验证合法性 */
    if(nCol < 0 || nRow < 0) return;
    if(nCol >= m_dwWidth || nRow >= m_dwHeight) return;

    /** 开始FloodFill */
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
    /** 计算当前鼠标所在位置的地图坐标 */
    int nMouseWorldX = m_nLeft + fMX;
    int nMouseWorldY = m_nTop + fMY;

    POINT tilePos = GetCoorOfTileByTrueWorld(nMouseWorldX, nMouseWorldY);
    int nCol = tilePos.x;
    int nRow = tilePos.y;

    /** 验证合法性 */
    if(nCol < 0 || nRow < 0) return;
    if(nCol >= m_dwWidth || nRow >= m_dwHeight) return;

    /** 开始FloodFill */
    bool* bVis = new bool[m_dwWidth * m_dwHeight];
    memset(bVis, 0, sizeof(bool) * m_dwWidth * m_dwHeight);
    POUMAP_CELL pCell = GetCellPtr(nCol, nRow);
    if(m_CurSelTile.nID == pCell->m_CarpetID) return;
    __floodfill(nCol, nRow, pCell->m_CarpetID, m_CurSelTile.nID, bVis, OUMAP_RES_TYPE::OUMRT_CARPET);
    delete []bVis;
    SetModified();
}

/** 这个不写大地图填充会爆栈，这个写了不能保存 */
//#pragma comment(linker,"/STACK:102400000,1024000")

void OUMapView::__floodfill(int x, int y, int from, int to, bool vis[], OUMAP_RES_TYPE type)
{
    /** 验证合法性 */
    if(x < 0 || y < 0) return;
    if(x >= m_dwWidth || y >= m_dwHeight) return;

    int idx = GetCellIdx(x, y);
    if(vis[idx]) return;

    if(type == OUMAP_RES_TYPE::OUMRT_TILE)
    {
        if(m_MapCell[idx].m_BottomID != from) return;

        /** 填充 */
        vis[idx] = true;
        m_MapCell[idx].m_BottomID = to;
        m_MapCell[idx].m_pBottomImg = GetCellImage(to, OUMAP_RES_TYPE::OUMRT_TILE);
    }
    else
    if(type == OUMAP_RES_TYPE::OUMRT_CARPET)
    {
        if(m_MapCell[idx].m_CarpetID != from) return;

        /** 填充 */
        vis[idx] = true;
        m_MapCell[idx].m_CarpetID = to;
        m_MapCell[idx].m_pCarpetImg = (to == OUMAP_TILE_BLACKID) ? NULL : (GetCellImage(to, OUMAP_RES_TYPE::OUMRT_CARPET));
    }

    /** 
     * 往邻接点填充
     *
     *   如果是偶数行，则邻接点是(x - 1, y ± 1), (x, y ± 1)
     *   如果是奇数行，则邻接点是(x, y ± 1), (x + 1, y ± 1)
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

    /** 如果鼠标按下了 */
    if(mousedown && lastmousedown)
    {
        OUPOINT pos = GetTruePosOfScreen();

        pos.m_fX -= (mx - lastmx);
        pos.m_fY -= (my - lastmy);

        MoveArea(pos.m_fX, pos.m_fY);

        TRACE("位置: (%d, %d)\n", (int)pos.m_fX, (int)pos.m_fY);
    }

    lastmx = mx, lastmy = my;
    lastmousedown = mousedown;
}

POINT OUMapView::GetCoorOfTileByTrueWorld(int xx, int yy)
{
    int nMouseWorldX = xx;
    int nMouseWorldY = yy;

    /**
     *  贴图方法：
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
    /** 偏左边的行数 */
    int nLeftRowCount = (m_dwHeight >> 1) + (m_dwHeight & 1);
    
    /** 在第几个偏左边行里 */
    int nLeftRow = nMouseWorldY / m_nTileHeight;
    
    /** 偏上还是偏下 */
    bool bUp = ((nMouseWorldY % m_nTileHeight) <= m_nHalfTileHeight);
    int RowMod = nMouseWorldY % m_nTileHeight;

    /** 在第几个纵格里 */
    int nColumn = nMouseWorldX / m_nTileWidth;

    /** 偏左还是偏右 */
    bool bLeft = ((nMouseWorldX % m_nTileWidth) <= m_nHalfTileWidth);
    int ColMod = nMouseWorldX % m_nTileWidth;
 
    /** 偏上且偏左 */
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
            /** 偏右边的上一行 */
            nCol = nColumn - 1;
            nRow = (nLeftRow << 1) - 1;
        }
        else
        {
            /** 偏左边的这一行 */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
    }
    else
    /** 偏上且偏右 */
    if(bUp && !bLeft)
    {
        /** y = (m_nHalfTileHeight / m_nHalfTileWidth) * x - m_nHalfTileHeight */
        /** y < (m_nHalfTileHeight / m_nHalfTileWidth) * x - m_nHalfTileHeight */
        /** y > (m_nHalfTileHeight / m_nHalfTileWidth) * x - m_nHalfTileHeight */
        if((y + m_nHalfTileHeight) * m_nHalfTileWidth < m_nHalfTileHeight * x)
        {
            /** 偏右边的上一行 */
            nCol = nColumn;
            nRow = (nLeftRow << 1) - 1;
        }
        else
        {
            /** 偏左边的这一行 */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
    }
    else
    /** 偏下且偏左 */
    if(!bUp && bLeft)
    {
        /** y = (m_nHalfTileHeight / m_nHalfTileWidth) * x + m_nHalfTileHeight */
        /** y < (m_nHalfTileHeight / m_nHalfTileWidth) * x + m_nHalfTileHeight */
        /** y > (m_nHalfTileHeight / m_nHalfTileWidth) * x + m_nHalfTileHeight */
        if((y - m_nHalfTileHeight) * m_nHalfTileWidth < m_nHalfTileHeight * x)
        {
            /** 偏左边的这一行 */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
        else
        {
            /** 偏右边的下一行 */
            nCol = nColumn - 1;
            nRow = (nLeftRow << 1) + 1;
        }
    }
    else
    /** 偏下且偏右 */
    if(!bUp && !bLeft)
    {
        /** y = 3 * m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        /** y < 3 * m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        /** y > 3 * m_nHalfTileHeight - (m_nHalfTileHeight / m_nHalfTileWidth) * x */
        if((3 * m_nHalfTileHeight - y) * m_nHalfTileWidth > m_nHalfTileHeight * x)
        {
            /** 偏左边的这一行 */
            nCol = nColumn;
            nRow = nLeftRow << 1;
        }
        else
        {
            /** 偏右边的下一行 */
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
    /** 越后面是越上层 */
    static hgeRect rect;
    for(list<POUMAP_UPPER_CELL>::reverse_iterator it = m_BuildingElement.rbegin(); it != m_BuildingElement.rend(); it++)
    {
        if(!(*it)->m_pImg) continue;
        OUPOINT sz = (*it)->m_pImg->GetSize();

        rect.Set((*it)->m_fTrueX, (*it)->m_fTrueY, (*it)->m_fTrueX + sz.m_fX, (*it)->m_fTrueY + sz.m_fY);
        if(rect.TestPoint(xx, yy))
        {
            /** 像素级检测 */
            if(!(*it)->m_pImg->m_bStatic) return (*it); ///< 动画的像素级检测以后再说
            else
            /** 桌子 */
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
    //static OUFont* font = new OUFont("文泉驿等宽微米黑", 12, 0, 0, 1);

    //m_pInnerFont->SetKerningWidth(1);
    //m_pInnerFont->SetKerningHeight(2);

    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);

    static char layname[][7] = {
        "地表层",
        "地毯层",
        "建筑层"
    };

    sprintf(info,
        "帧　　率: %d FPS\n"              \
        "时间间隔: %.6f\n"                \
        "鼠标位置: (%.4d, %.4d)\n"        \
        "渲染图块: %d\n\n"                \
        "选择图层: %s\n"                  \
        "选择图块: %.5d\n",
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
    /** 大小序号弄好 */
    __orderswap2(x1, x2);
    __orderswap2(y1, y2);

    float sx1, sy1, sx2, sy2;
    sx1 = m_nLeft;
    sy1 = m_nTop;
    sx2 = m_nLeft + m_nScreenWidth.GetValue();
    sy2 = m_nTop + m_nScreenWidth.GetValue();

    /** 没相交——包含 */
    if(sx1 <= x1 && sy1 <= y1 && sx2 >= x2 && sy2 >= y2) return true;
    if(sx1 >= x1 && sy1 >= y1 && sx2 <= x2 && sy2 <= y2) return true;

    hgeRect rc(x1, y1, x2, y2);
    hgeRect screenrc(sx1, sy1, sx2, sy2);

    /** 有相交 */
    if(rc.Intersect(&screenrc)) return true;
    
    /** 在外面 */
    return false;
}

void OUMapView::SetScreenSize(int nScreenWidth, int nScreenHeight)
{
    m_nScreenWidth = nScreenWidth;
    m_nScreenHeight = nScreenHeight;
}

void OUMapView::RecreateTarget(int nWidth, int nHeight)
{
    /** 删除原精灵 */
    if(m_pMapSprite)
    {
        delete m_pMapSprite;
        m_pMapSprite = NULL;
    }

    /** 释放原目标 */
    if(m_hMapTarget)
    {
        m_pHGE->Target_Free(m_hMapTarget);
        m_hMapTarget = 0;
    }

    /** 创建新目标和精灵 */
    m_hMapTarget = m_pHGE->Target_Create(nWidth, nHeight, true);
    m_pMapSprite = new hgeSprite(m_pHGE->Target_GetTexture(m_hMapTarget), 0, 0, nWidth, nHeight);
}

void OUMapView::RebuildTarget(float fDelta)
{
    if(m_hMapTarget == 0)
    {
        TRACE0("地图无渲染目标。\n");
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

    /** 若鼠标块需要更新 */
    if(m_CurSelTile.pImg && !m_CurSelTile.pImg->m_bStatic)
    {
        updated[m_CurSelTile.nID] = true;
        m_CurSelTile.pImg->GetAnimation()->Update(fDelta);
    }

    /** 从左上角开始找 */
    for(int i = 0; i < m_dwHeight; i++)
    {
        /** 此行贴图的y起始位置 */
        ty = i * m_nHalfTileHeight;
        
        /** 若起始位置不够则继续 */
        if(ty + m_nTileHeight <= m_nTop) continue;
        
        /** 若起始位置超了则跳出 */
        if(ty >= m_nTop + GetScreenHeight()) break;
        
        /** 枚举本行各列贴图 */
        for(int j = 0; j < m_dwWidth; j++)
        {
            /** 此贴图x位置 */
            tx = j * m_nTileWidth;
            if(i % 2 == 1) tx += m_nHalfTileWidth;
            
            /** 若起始x不够则继续 */
            if(tx + m_nTileWidth <= m_nLeft) continue;
            
            /** 若起始位置超了则跳出 */
            if(tx >= m_nLeft + GetScreenWidth()) break;
            
            /** 获取贴图信息 */
            OUMAP_CELL cell = GetCell(j, i);
            
            if(cell.m_pBottomImg != NULL)
            {
                /** 若是动态贴图则更新动画 */
                if(!cell.m_pBottomImg->m_bStatic && !updated[cell.m_BottomID])
                {
                    updated[cell.m_BottomID] = true;
                    cell.m_pBottomImg->GetAnimation()->Update(fDelta);
                }
                
                /** 渲染贴图到目标 */
                cell.m_pBottomImg->RenderStretch(tx - m_nLeft, ty - m_nTop, m_nTileWidth, m_nTileHeight);

                /** 渲染图块加一 */
                m_dwRenderCount++;
            }

            if(cell.m_pCarpetImg != NULL)
            {
                /** 若是动态贴图则更新动画 */
                if(!cell.m_pCarpetImg->m_bStatic && !carupdated[cell.m_CarpetID])
                {
                    carupdated[cell.m_CarpetID] = true;
                    cell.m_pCarpetImg->GetAnimation()->Update(fDelta);
                }

                /** 渲染贴图到目标 */
                cell.m_pCarpetImg->RenderStretch(tx - m_nLeft, ty - m_nTop, m_nTileWidth, m_nTileHeight);

                /** 渲染图块加一 */
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

    /** 建筑 */
    OUPOINT sz;
    for(list<POUMAP_UPPER_CELL>::iterator it = m_BuildingElement.begin(); it != m_BuildingElement.end(); it++)
    {
        /** 宽高 */
        if(!(*it)->m_pImg) continue;
        OUPOINT sz = (*it)->m_pImg->GetSize();

        /** 不在里面 */
        if(!TestRectInScreen((*it)->m_fTrueX, (*it)->m_fTrueY, (*it)->m_fTrueX + sz.m_fX, (*it)->m_fTrueY + sz.m_fY))
        {
            continue;
        }

        /** 如果是焦点图像 */
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
        /** 如果是焦点被选中图像 */
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

        /** 渲染图块加一 */
        m_dwRenderCount++;
    }

    /** 上层网格 */
    if(m_bUpperGrid.GetValue()) DrawUpperGrid();

    m_pHGE->Gfx_EndScene();

    m_pHGE->System_SetState(HGE_TEXTUREFILTER, true);
}

void OUMapView::DrawUpperGrid()
{
    POUGUI_IMG pGroundImg = GetCellImage(OUMAP_TILE_GROUND, OUMRT_TILE);
    POUGUI_IMG pFocusImg = GetCellImage(OUMAP_TILE_FOCUS, OUMRT_TILE);
    POUGUI_IMG pBlockImg = GetCellImage(OUMAP_TILE_BLOCK, OUMRT_TILE);

    /** 鼠标所在 */
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

            /** 如果是障碍物模式 */
            OUMAP_TILE_COOR coor = { j, i };
            if(m_emEditMode == OUMEM_BLOCK)
            {
                /** 如果是障碍物 */
                pCell = GetCellPtr(j, i);
                if(NULL != pCell && pCell->block && pBlockImg)
                {
                    pBlockImg->Render(tx - m_nLeft, ty - m_nTop);
                }
                else
                /** 否则如果是鼠标悬浮点 */
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
            /** 如果是地基模式 */
            if(m_emEditMode == OUMEM_GROUND)
            {
                /** 如果有选中的 */
                if(OUMAP_UPPER_CELL::m_pClickFocused)
                {
                    /** 如果这个坐标是地基 */
                    OUMAP_TILE_COOR coor = { j, i };
                    if(OUMAP_UPPER_CELL::m_pClickFocused->m_pGroundSet->find(coor) != OUMAP_UPPER_CELL::m_pClickFocused->m_pGroundSet->end())
                    {
                        if(pGroundImg)
                        {
                            pGroundImg->Render(tx - m_nLeft, ty - m_nTop);
                        }
                    }
                    /** 否则如果是鼠标悬浮点 */
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
    /** 删除原精灵 */
    if(m_pMapSprite)
    {
        delete m_pMapSprite;
        m_pMapSprite = NULL;
    }

    /** 释放原目标 */
    if(m_hMapTarget)
    {
        m_pHGE->Target_Free(m_hMapTarget);
    }
}

POUGUI_IMG OUMapView::GetCellImage(int id, OUMAP_RES_TYPE type)
{
    switch(type)
    {
        /** 地底层 */
    case OUMRT_TILE:
        {
            if(m_Tiles.find(id) == m_Tiles.end())
            {
                /** 若没找到则先载入 */
                LoadImage(id, type);
            }

            /** 若还没找到则真找不到了 */
            if(m_Tiles.find(id) == m_Tiles.end()) return NULL;
            
            return &m_Tiles[id].img;

            break;
        }

        /** 地毯层 */
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

        /** 建筑层 */
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

        /** 桌子层 */
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
        /** 贴图 */
    case OUMRT_TILE:
        {
            pContainer = &m_Tiles;
            sprintf(prefix, OUMAP_TILE_NAME_FORMAT, id);

            /** 地基贴图 */
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

        /** 地毯 */
    case OUMRT_CARPET:
        {
            pContainer = &m_CarpetRes;
            sprintf(prefix, OUMAP_CARPET_NAME_FORMAT, id);
            break;
        }

        /** 建筑 */
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

    /** 根据各后缀进行枚举查找 */
    for(int i = 0; i < __OUMAPTILESUFFIX_COUNT; i++)
    {
        string filename = prefix + __OUMAPTILESUFFIX[i];

        /** 试着查找资源 */
        OUPACKFILEDATA data = OUResourcePool::instance().GetResource(filename);
        if(data.m_dwFileSize == 0) continue;

        /** 如果是动画地板 */
        if(".oga" == __OUMAPTILESUFFIX[i])
        {
            OUMAP_TEX_REF ref;
            ref.id = id;
            ref.img.m_bStatic = false;
            ref.tex = 0;
            
            /** 若载入失败则继续 */
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
        /** 非动画 */
        {
            OUMAP_TEX_REF ref;
            ref.id = id;
            ref.img.m_bStatic = true;
            ref.tex = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize, true);

            if(ref.tex == 0) continue;

            /** 真实宽高 */
            int iw, ih;
            iw = m_pHGE->Texture_GetWidth(ref.tex, true);
            ih = m_pHGE->Texture_GetHeight(ref.tex, true);

            /** 2的次幂宽高——用于锁资源 2012/12/5 23:57 */
            int fw, fh;
            fw = m_pHGE->Texture_GetWidth(ref.tex);
            fh = m_pHGE->Texture_GetHeight(ref.tex);

            /** 透明色 */
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

    /** 不能显示左边和右边的尖 --- 但是编辑器可以显示 */
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

    /** 不能显示上下的尖 --- 但是编辑器可以显示 */
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

    /** 重构地图贴图 */
    //RebuildTile(fDelta);

    if(m_nLeft == left && m_nTop == top) return false;
    else
    {
        /** 若不是因滚动条移动的则滚动条滚动 */
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
        /** 窗体大小改变 */
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

        /** 新建地图消息 */
    case OM_NEWMAP:
        {
            OM_NEWMAP_PARAM* param = (OM_NEWMAP_PARAM*)Msg.lpParam;
            NewMap(param->m_szName, param->m_nWidth, param->m_nHeight, param->m_nTileWidth, param->m_nTileHeight);
            break;
        }

        /** 修改当前方块 */
    case OM_SELECTTILE:
        {
            OM_SELECTTILE_PARAM* param = (OM_SELECTTILE_PARAM*)Msg.lpParam;

            /** 地底层 */
            if(param->m_emResType == OUMRT_TILE)
            {
                m_CurSelTile.emType = (OUMAP_RES_TYPE)param->m_emResType;
                m_CurSelTile.nID = param->m_nID;

                m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
            }
            else
            /** 地毯层 */
            if(param->m_emResType == OUMRT_CARPET)
            {
                m_CurSelTile.emType = (OUMAP_RES_TYPE)param->m_emResType;
                m_CurSelTile.nID = param->m_nID;

                m_CurSelTile.pImg = GetCellImage(m_CurSelTile.nID, m_CurSelTile.emType);
            }
            else
            /** 建筑层 */
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

        /** 修改编辑模式参数 */
    case OM_CHANGEEDITMODE:
        {
            OM_CHANGEEDITMODE_PARAM* param = (OM_CHANGEEDITMODE_PARAM*)Msg.lpParam;

            /** 如果原先是能选择层而现在不是则清除焦点 */
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

        /** 滚动条移动地图 */
    case OM_SCROLLMOVEAREA:
        {
            OM_SCROLLMOVEAREA_PARAM* param = (OM_SCROLLMOVEAREA_PARAM*)Msg.lpParam;

            MoveArea(param->m_nToX, param->m_nToY, true);
            break;
        }

        /** 改变内部字体 */
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
