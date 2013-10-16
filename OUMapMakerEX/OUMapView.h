#ifndef __OUMAPVIEW_H__
#define __OUMAPVIEW_H__
#pragma once
#include "Global.h"
#include "OUMapObject.h"
#include "../OUPlazaRender/OUResourceManager.h"
using namespace std;

//#define OUMAP_TILE_WIDTH            64
//#define OUMAP_TILE_HEIGHT           32
//#define HALF_TILE_WIDTH             (OUMAP_TILE_WIDTH / 2)
//#define HALF_TILE_HEIGHT            (OUMAP_TILE_HEIGHT / 2)

#define DEFAULT_MAP_WIDTH           25
#define DEFAULT_MAP_HEIGHT          50
#define DEFAULT_TILE_WIDTH          64
#define DEFAULT_TILE_HEIGHT         32

#define OUMAP_TILE_BLACKID          0
#define OUMAP_TILE_FOCUS            -1
#define OUMAP_TILE_GROUND           -2
#define OUMAP_TILE_BLOCK            -3

#define OUMAP_CURSOR_SIZE           32

#define OUMAP_MAGIC_NUMBER          0xC2D4C0CB

enum OUMAP_RES_TYPE
{
    OUMRT_TILE,
    OUMRT_CARPET,
    OUMRT_BUILDING,
    OUMRT_TABLE,

    OUMRT_MAX
};

enum OUMAP_EDIT_MODE
{
    OUMEM_MOVE,
    OUMEM_ADD,
    OUMEM_FILL,
    OUMEM_ERASE,
    OUMEM_CHOOSE,
    OUMEM_GROUND,
    OUMEM_BLOCK,

    OUMEM_MAX
};

/** ����Ԫ�� */
typedef struct tagOUMAP_CELL
{
    DWORD                       m_BottomID;
    POUGUI_IMG                  m_pBottomImg;

    DWORD                       m_CarpetID;
    POUGUI_IMG                  m_pCarpetImg;

    bool                        block;

    int                         x;
    int                         y;
} OUMAP_CELL, *POUMAP_CELL;

typedef struct tagOUMAP_CELL_TOSAVE
{
    DWORD                       m_dwBottomID;
    DWORD                       m_dwCarpetID;
    bool                        m_bBlock;
} OUMAP_CELL_TOSAVE, *POUMAP_CELL_TOSAVE;

/** ������Ԫ�� */
typedef class tagOUMAP_UPPER_CELL : public OUMapObject
{
public:
    OUMAP_RES_TYPE              m_emType;

    DWORD                       m_dwID;
    POUGUI_IMG                  m_pImg;

    float                       m_fTrueX;
    float                       m_fTrueY;

    static tagOUMAP_UPPER_CELL* m_pFocused;
    static tagOUMAP_UPPER_CELL* m_pClickFocused;

    set<OUMAP_TILE_COOR>*       m_pGroundSet;
    bool                        m_bAutoDelGroundSet;

    //void                        SetAutoDel(bool bAuto)
    //{
    //    m_bAutoDelGroundSet = bAuto;
    //}

public:
    //void copy(tagOUMAP_UPPER_CELL& cell)
    //{
    //    if(m_bAutoDelGroundSet)
    //    {
    //        SAFEDEL(m_pGroundSet);
    //    }

    //    m_emType = cell.m_emType;
    //    m_dwID = cell.m_dwID;
    //    m_fTrueX = cell.m_fTrueX;
    //    m_fTrueY = cell.m_fTrueY;
    //    m_pImg = cell.m_pImg;
    //    m_emObjectType = OMOT_BUILDING;
    //    m_bAutoDelGroundSet = true;
    //    m_pGroundSet = cell.m_pGroundSet;
    //    
    //    cell.SetAutoDel(false);
    //}

    tagOUMAP_UPPER_CELL()
    {
        m_bAutoDelGroundSet = true;
        m_emType = OUMRT_MAX;
        m_dwID = 0;

        m_fTrueX = m_fTrueY = 0;

        /** Ԫ�ز����� */
        m_emObjectType = OMOT_BUILDING;

        m_pGroundSet = new set<OUMAP_TILE_COOR>();
    }

    ~tagOUMAP_UPPER_CELL()
    {
        if(m_bAutoDelGroundSet)
        {
            m_pGroundSet->clear();
            delete m_pGroundSet;
        }
    }
} OUMAP_UPPER_CELL, *POUMAP_UPPER_CELL;

typedef struct tagOUMAP_UPPER_CELL_TOSAVE
{
    DWORD                       m_Order;
    OUMAP_RES_TYPE              m_emType;

    DWORD                       m_dwID;

    float                       m_fTrueX;
    float                       m_fTrueY;

    DWORD                       m_dwCoverCount;
} OUMAP_UPPER_CELL_TOSAVE, *POUMAP_UPPER_CELL_TOSAVE;

class OUMapTableImg;

/** �����ļ����� */
typedef struct tagOUMAP_TEX_REF
{
    DWORD                       id;
    HTEXTURE                    tex;
    OUGUI_IMG                   img;
    OUMapTableImg*              pImg;

    float                       width;
    float                       height;

    tagOUMAP_TEX_REF()
    {
        id = 0, tex = 0;
        img.m_bStatic = true;
        img.m_Img.m_pSprite = NULL;
        pImg = NULL;

        width = height = 0;
    }
} OUMAP_TEX_REF, *POUMAP_TEX_REF;

/** ��Ϣ���ж��� */
typedef struct tagOUMAP_MSG_QUEUE_ITEM
{
    OUMAPVIEWMSG                emMsg;
    LPVOID                      lpParam;
    bool                        bAutoDel;
} OUMAP_MSG_QUEUE_ITEM, *POUMAP_MSG_QUEUE_ITEM;

typedef struct tagOUMAP_CURTILE
{
    OUMAP_RES_TYPE              emType;
    int                         nID;
    POUGUI_IMG                  pImg;
} OUMAP_CURTILE, *POUMAP_CURTILE;

typedef struct tagOUMAP_MAPFILE_HEADER
{
    DWORD                       m_dwMagicNum;
    char                        m_szMapName[64];

    DWORD                       m_dwTileWidth;
    DWORD                       m_dwTileHeight;

    DWORD                       m_dwMapWidth;
    DWORD                       m_dwMapHeight;

    /** ������ */
    DWORD                       m_dwObjectCount;

    /** ͷ��С */
    DWORD                       m_dwHeaderSize;
} OUMAP_MAPFILE_HEADER, *pOUMAP_MAPFILE_HEADER;

class OUMapView
{
private:
    /** �ײ���ͼ�ߴ� */
    static int                  m_nTileWidth;//OUMAP_TILE_WIDTH;
    static int                  m_nTileHeight;//OUMAP_TILE_HEIGHT;
    static int                  m_nHalfTileWidth;//HALF_TILE_WIDTH;
    static int                  m_nHalfTileHeight;//HALF_TILE_HEIGHT;

    //string                             m_szBasePath;

public:
    OUMapView(void);
    virtual ~OUMapView(void);

    void                        OnUpdate(float fDelta);
    void                        Render();

    /** �½���ͼ */
    void                        NewMap(const char* name, int w, int h, int tw, int th);
    /** �رյ�ͼ */
    void                        CloseMap();
    /** �����ͼ */
    BOOL                        SaveMap(LPCTSTR szFilename);
    /** �򿪵�ͼ */
    BOOL                        OpenMap(LPCTSTR szFilename);

    /** ��ȡ��ʾ״̬ */
    bool                        GetShowState()                      { return m_bShowState.GetValue(); }
    void                        SetShowState(bool val)              { m_bShowState.SetValue(val); }
    bool                        GetShowGrid()                       { return m_bShowGrid.GetValue(); }
    void                        SetShowGrid(bool val)               { m_bShowGrid.SetValue(val); }
    bool                        GetUpperGrid()                      { return m_bUpperGrid.GetValue(); }
    void                        SetUpperGrid(bool val)              { m_bUpperGrid.SetValue(val); }
    void                        RenderState();
    /** �ƶ���ͼ */
    bool                        MoveArea(int dwTrueX, int dwTrueY, bool bScrollBy = false);

    /** ������ʵ�����С */
    void                        CalcTrueWorldSize();
    static SIZE                 GetDocSize(int nWidth, int nHeight, int nTileWidth, int nTileHeight);
    
    /** �ж�ĳ�������Ƿ�����Ļ��ͼ���� */
    bool                        TestRectInScreen(float x1, float y1, float x2, float y2);
    bool                        TestRectInScreen(CRect& rect);

    /** ��ͼ��С */
    int                         GetMapWidth()                       { return m_dwWidth; }
    int                         GetMapHeight()                      { return m_dwHeight; }
    int                         GetTileWidth()                      { return m_nTileWidth; }
    int                         GetTileHeight()                     { return m_nTileHeight; }

    /** ��Ļ��С */
    void                        SetScreenSize(int nScreenWidth, int nScreenHeight);
    int                         GetScreenWidth()                    { return m_nScreenWidth.GetValue(); }
    int                         GetScreenHeight()                   { return m_nScreenHeight.GetValue(); }
    SIZE                        GetScreenSize()                     { SIZE sz; sz.cx = m_nScreenWidth.GetValue(); sz.cx = m_nScreenHeight.GetValue(); return sz; }
    OUPOINT                     GetTruePosOfScreen()                { OUPOINT pt; pt.m_fX = m_nLeft, pt.m_fY = m_nTop; return pt; }
    /** �ж�ĳһ����ʵ�����ڵ�ͼ�ĵڼ�����ͼ�� */
    POINT                       GetCoorOfTileByTrueWorld(int xx, int yy);
    /** ��ȡ�������λ�õĵ�һ����ͼ��� */
    POUMAP_UPPER_CELL           GetFirstMapObjectByMouse(int xx, int yy);

    /** �ؽ�Target */
    void                        RecreateTarget(int nWidth, int nHeight);
    /** �ػ�Target */
    void                        RebuildTarget(float fDelta);
    /** ���ϲ����� */
    void                        DrawUpperGrid();
    /** ���Target */
    void                        ClearTarget();

    /** ��ȡ����ͼƬ */
    POUGUI_IMG                  GetCellImage(int id, OUMAP_RES_TYPE type);
    bool                        LoadImage(int id, OUMAP_RES_TYPE type);
    /** ��ȡ��ͼԪ�� */
    OUMAP_CELL                  GetCell(int x, int y)               { return m_MapCell[y * m_dwWidth + x]; }
    POUMAP_CELL                 GetCellPtr(int x, int y)            { return &m_MapCell[y * m_dwWidth + x]; }
    inline int                  GetCellIdx(int x, int y)            { return y * m_dwWidth + x; }

    /** �̰߳�ȫ��������Ϣ���� */
    void                        SendMessage(OUMAPVIEWMSG Msg, LPVOID lpParam, bool bAutoDel = true);
    /** ������Ϣ */
    void                        TranslateMessage(OUMAP_MSG_QUEUE_ITEM Msg);

    /** �����ĵ����޸� */
    void                        SetModified(bool flag = true);

private:
    void                        DoEditMap();
    void                        DoGround();
    void                        DoBlock();
    void                        MoveMapByMouse();

    /** �����ͼ */
    void                        AddTile(float fMX, float fMY);
    void                        AddBottomTile(float fMX, float fMY);
    void                        AddCarpetTile(float fMX, float fMY);
    void                        AddBuildingTile(float fMX, float fMY);
    void                        AddTableTile(float fMX, float fMY);
    /** �����ͼ */
    void                        FillBottomTile(float fMX, float fMY);
    void                        FillCarpetTile(float fMX, float fMY);
    void                        __floodfill(int x, int y, int from, int to, bool vis[], OUMAP_RES_TYPE type);
    /** �Ƴ���ͼ */
    void                        EraseTile(float fMX, float fMY);
    void                        EraseBottomTile(float fMX, float fMY);
    void                        EraseObject(float fMX, float fMY);
    /** ѡ��ģʽ */
    void                        ChooseMapObject();

    /** ���������� */
    void                        DrawSingleGrid(int sx, int sy, DWORD color);

private:
    /** ��ͼ�� */
    string                      m_szName;

    HGE*                        m_pHGE;

    int                         m_dwWidth;
    int                         m_dwHeight;
    int                         m_dwTrueWidth;
    int                         m_dwTrueHeight;

    int                         m_nTop;
    int                         m_nLeft;

    /** �༭ģʽ */
    OUMAP_EDIT_MODE             m_emEditMode;

    /** ��Ļ��С */
    OUSafeObj<int>              m_nScreenWidth;
    OUSafeObj<int>              m_nScreenHeight;

    /** ��ǰ��Ļ��ͼ�� */
    DWORD                       m_dwRenderCount;

    /** ��ǰѡ��ͼ�� */
    OUMAP_CURTILE               m_CurSelTile;

    /** �����Լ�Target */
    HTARGET                     m_hMapTarget;
    hgeSprite*                  m_pMapSprite;

    /** �Ƿ���ʾ�༭��״̬��Ϣ */
    OUSafeObj<bool>             m_bShowState;
    /** �Ƿ���ʾ������ */
    OUSafeObj<bool>             m_bShowGrid;
    /** �Ƿ���ʾ�ϲ����� */
    OUSafeObj<bool>             m_bUpperGrid;

    /** ��ͼԪ�� */
    vector<OUMAP_CELL>          m_MapCell;
    list<POUMAP_UPPER_CELL>     m_BuildingElement;
    //list<OUMAP_UPPER_CELL>      m_CarpetElement;

    /** �����ز� */
    map<int, OUMAP_TEX_REF>     m_Tiles;
    map<int, OUMAP_TEX_REF>     m_CarpetRes;
    map<int, OUMAP_TEX_REF>     m_BuildingRes;
    map<int, OUMAP_TEX_REF>     m_Table;

    /** ��Ϣ���� */
    queue<OUMAP_MSG_QUEUE_ITEM> m_MsgQueue;
    ::CRITICAL_SECTION          m_MsgQueueCS;

    /** ��� */
    hgeSprite*                  m_pCursor[OUMEM_MAX];
    HTEXTURE                    m_hCursor[OUMEM_MAX];
    hgeSprite*                  m_pCantCursor;
    HTEXTURE                    m_hCantCursor;

    /** �ڲ����� */
    OUFont*                     m_pInnerFont;
};

#endif
