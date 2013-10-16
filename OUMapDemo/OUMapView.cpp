#include "StdAfx.h"
#include "OUMapView.h"

OUMapView::OUMapView() :
    m_pHGE(NULL),
    m_pReader(NULL),
    m_hMapTarget(0),
    m_pMapSprite(NULL),
    m_fWorldLeft(0.0f),
    m_fWorldTop(0.0f)
{
    m_pHGE = hgeCreate(HGE_VERSION);

    hgeRect rect(0, 0, 800, 600);
    SetMapScreen(rect);

    m_pResMgr = new OUMapResourceMgr();
}

OUMapView::~OUMapView(void)
{
    Clear();

    ReleaseTargetAndSprite();
    SAFEDEL(m_pReader);
    SAFEDEL(m_pResMgr);

    m_pHGE->Release();
    m_pHGE = NULL;
}

void OUMapView::MoveBy(float x, float y)
{
    MoveTo(m_fWorldLeft + x, m_fWorldTop + y);
}

void OUMapView::MoveTo(float fLeft, float fTop)
{
    float left, top;
    left = m_fWorldLeft;
    top = m_fWorldTop;

    float fScreenWidth = m_rtMapScreen.GetWidth();
    float fScreenHeight = m_rtMapScreen.GetHeight();

    float fTrueWidth = m_pReader->GetWorldWidth();
    float fTrueHeight = m_pReader->GetWorldHeight();

    if(fTrueWidth - m_pReader->GetTileWidth() < fScreenWidth)
    {
        m_fWorldLeft = -(fScreenWidth - fTrueWidth) / 2;
    }
    else
    {
        if(fLeft < m_pReader->GetHalfTileWidth())
        {
            fLeft = m_pReader->GetHalfTileWidth();
        }

        if(fLeft + fScreenWidth + m_pReader->GetHalfTileWidth() > fTrueWidth)
        {
            fLeft = fTrueWidth - fScreenWidth - m_pReader->GetHalfTileWidth();
        }

        m_fWorldLeft = fLeft;
    }

    if(fTrueHeight - m_pReader->GetTileHeight() < fScreenHeight)
    {
        m_fWorldTop = -(fScreenHeight - fTrueHeight) / 2;
    }
    else
    {
        if(fTop < m_pReader->GetHalfTileHeight())
        {
            fTop = m_pReader->GetHalfTileHeight();
        }

        if(fTop + fScreenHeight + m_pReader->GetHalfTileHeight() > fTrueHeight)
        {
            fTop = fTrueHeight - fScreenHeight - m_pReader->GetHalfTileHeight();
        }
        
        m_fWorldTop = fTop;
    }
}

bool OUMapView::SetMap(const char *filename)
{
    if(!m_pReader) m_pReader = new OUMapReader();

    if(m_pReader->Open(filename))
    {
        ReaderToThis();

        MoveTo(0, 0);
        return true;
    }

    return false;
}

void OUMapView::Clear()
{
    memset(m_nTileArray, 0, sizeof(m_nTileArray));
    memset(m_nCarpetArray, 0, sizeof(m_nCarpetArray));

    for(int i = 0; i < m_pBuildings.size(); i++)
    {
        SAFEDEL(m_pBuildings[i]);
    }
    m_pBuildings.clear();

    OUMapResourceTable::ReleaseAllImage();
}

void OUMapView::ReaderToThis()
{
    if(!m_pReader->IsOpened()) return;

    Clear();
    m_nWidth = m_pReader->GetWidth();
    m_nHeight = m_pReader->GetHeight();
    for(int i = 0; i < m_pReader->GetHeight(); i++)
    {
        for(int j = 0; j < m_pReader->GetWidth(); j++)
        {
            m_nTileArray[i][j] = m_pReader->GetCellInfo(j, i).m_dwBottomID;
            m_nCarpetArray[i][j] = m_pReader->GetCellInfo(j, i).m_dwCarpetID;
            m_bBlockArray[i][j] = m_pReader->GetCellInfo(j, i).m_bBlock;
        }
    }

    m_AStar.SetMap(m_pReader->GetWidth(), m_pReader->GetHeight(), this);

    for(int i = 0; i < m_pReader->GetBuildingCount(); i++)
    {
        OUMapBuildingInfo info;
        info = m_pReader->GetBuildingInfo(i);

        if(info.m_dwID == 5)
        {
            int test = 0;
        }

        switch(info.m_emType)
        {
        case OUMAP_RES_TYPE::OUMRT_BUILDING:
            {
                OUMapItemBuilding* building = new OUMapItemBuilding();
                building->SetBuildingInfo(info);
                m_pBuildings.push_back(building);

                break;
            }

        case OUMAP_RES_TYPE::OUMRT_TABLE:
            {
                OUMapItemTable* table = new OUMapItemTable();
                table->SetBuildingInfo(info);
                m_pBuildings.push_back(table);

                break;
            }

        default: break;
        }
    }
}

void OUMapView::SetMapScreen(hgeRect &rect)
{
    m_rtMapScreen = rect;
    CreateTargetAndSprite();
}

hgeRect OUMapView::GetMapRect()
{
    return m_rtMapScreen;
}

void OUMapView::RestoreTarget()
{
    if(m_hMapTarget && m_pMapSprite)
    {
        m_pMapSprite->SetTexture(m_pHGE->Target_GetTexture(m_hMapTarget));
    }
}

bool OUMapView::CreateTargetAndSprite()
{
    ReleaseTargetAndSprite();

    if(m_rtMapScreen.x2 - m_rtMapScreen.x1 <= 0) return false;
    if(m_rtMapScreen.y2 - m_rtMapScreen.y1 <= 0) return false;

    m_hMapTarget = m_pHGE->Target_Create(m_rtMapScreen.x2 - m_rtMapScreen.x1, m_rtMapScreen.y2 - m_rtMapScreen.y1, true);
    m_pMapSprite = new hgeSprite(m_pHGE->Target_GetTexture(m_hMapTarget), 0, 0, m_rtMapScreen.x2 - m_rtMapScreen.x1, m_rtMapScreen.y2 - m_rtMapScreen.y1);

    return true;
}

void OUMapView::ReleaseTargetAndSprite()
{
    SAFEDEL(m_pMapSprite);
    if(0 != m_hMapTarget)
    {
        m_pHGE->Target_Free(m_hMapTarget);
        m_hMapTarget = 0;
    }
}

hgeSprite* OUMapView::GetSprite()
{
    return m_pMapSprite;
}

void OUMapView::Update(float fMX, float fMY, float fDelta)
{
    /** 找桌子 */
    static OUMapItemTable* table = NULL;
    for(int i = 0; i < m_pBuildings.size(); i++)
    {
        if(m_pBuildings[i]->GetObjectType() == OUMAPITEM_TYPE::OUMIT_TABLE)
        {
            if(!m_pBuildings[i]->IsInsideScreen(m_fWorldLeft, m_fWorldTop, m_rtMapScreen.GetWidth(), m_rtMapScreen.GetHeight()))
            {
                continue;
            }

            table = (OUMapItemTable*)m_pBuildings[i];
            table->ResponseInput(fMX - (m_pBuildings[i]->GetX() - m_fWorldLeft), fMY - (m_pBuildings[i]->GetY() - m_fWorldTop));
        }
    }

    /** 人物输入 */
    if(m_pHGE->Input_IsMouseOver() && m_pHGE->Input_GetKeyState(HGEK_RBUTTON))
    {
        m_pMyRole->Goto(fMX + m_fWorldLeft, fMY + m_fWorldTop);
    }

    /** 人物动画 */
    if(m_pMyRole) m_pMyRole->Update(fDelta);
    for(int i = 0; i < m_pRoles.size(); i++)
    {
        m_pRoles[i]->Update(fDelta);
    }
}

bool OUMapView::RebuildTarget(float fDeltaTime)
{
    if(!m_hMapTarget) return false;

    m_pHGE->Gfx_BeginScene(m_hMapTarget);
    m_pHGE->Gfx_Clear(0xff000000);

    m_pHGE->System_SetState(HGE_TEXTUREFILTER, false);
    RerenderTileLayer(fDeltaTime);
    
    RerenderCarpetLayer(fDeltaTime);
    m_pHGE->System_SetState(HGE_TEXTUREFILTER, true);

    RerenderBuildingLayer(fDeltaTime);
    RerenderRoleAndCover(fDeltaTime);

    m_pHGE->Gfx_EndScene();

    return true;
}

void OUMapView::RerenderTileLayer(float fDeltaTime)
{
    if(!m_pReader->IsOpened()) return;
    static int nLeft;
    static int nTop;
    int tx, ty;

    nLeft = m_fWorldLeft;
    nTop = m_fWorldTop;

    static bool updated[65536];
    memset(updated, 0, sizeof(updated));

    for(int i = 0; i < m_pReader->GetHeight(); i++)
    {
        ty = i * m_pReader->GetHalfTileHeight();
        if(ty + m_pReader->GetTileHeight() <= nTop) continue;
        if(ty >= nTop + m_rtMapScreen.GetHeight()) break;

        for(int j = 0; j < m_pReader->GetWidth(); j++)
        {
            tx = j * m_pReader->GetTileWidth();
            if(i & 1) tx += m_pReader->GetHalfTileWidth();

            if(tx + m_pReader->GetTileWidth() <= nLeft) continue;
            if(tx >= nLeft + m_rtMapScreen.GetWidth()) break;

            //OUMapCellInfo info = m_pReader->GetCellInfo(j, i);
            OUMapResourceTile* tile = m_pResMgr->GetTileImage(m_nTileArray[i][j]);
            if(tile == NULL) continue;
            tile->SetZ(1.0f);

            if(!updated[m_nTileArray[i][j]])
            {
                updated[m_nTileArray[i][j]] = true;
                tile->Update(fDeltaTime);
            }

            if(sgn(tile->GetWidth() - m_pReader->GetTileWidth()) ||
                sgn(tile->GetHeight() - m_pReader->GetTileHeight()))
            {
                tile->RenderStretch(tx - m_fWorldLeft, ty - m_fWorldTop, m_pReader->GetTileWidth(), m_pReader->GetTileHeight());
            }
            else
            {
                tile->Render(tx - m_fWorldLeft, ty - m_fWorldTop);
            }

            //this->DrawSingleGrid(tx - m_fWorldLeft, ty - m_fWorldTop, 0xffff0000);
            //OUFontLoader::instance().GetFont(string("宋体"))->Print(tx - m_fWorldLeft + 10, ty - m_fWorldTop + 12, "(%d, %d)", j, i);
        }
    }
}

void OUMapView::RerenderCarpetLayer(float fDeltaTime)
{
    if(!m_pReader->IsOpened()) return;
    static int nLeft;
    static int nTop;
    int tx, ty;

    nLeft = m_fWorldLeft;
    nTop = m_fWorldTop;

    static bool updated[65536];
    memset(updated, 0, sizeof(updated));

    for(int i = 0; i < m_pReader->GetHeight(); i++)
    {
        ty = i * m_pReader->GetHalfTileHeight();
        if(ty + m_pReader->GetTileHeight() <= nTop) continue;
        if(ty >= nTop + m_rtMapScreen.GetHeight()) break;

        for(int j = 0; j < m_pReader->GetWidth(); j++)
        {
            tx = j * m_pReader->GetTileWidth();
            if(i & 1) tx += m_pReader->GetHalfTileWidth();

            if(tx + m_pReader->GetTileWidth() <= nLeft) continue;
            if(tx >= nLeft + m_rtMapScreen.GetWidth()) break;

            //OUMapCellInfo info = m_pReader->GetCellInfo(j, i);
            if(m_nCarpetArray[i][j] == 0) continue;

            OUMapResourceCarpet* tile = m_pResMgr->GetCarpetImage(m_nCarpetArray[i][j]);
            if(tile == NULL) continue;
            tile->SetZ(0.9f);

            if(!updated[m_nCarpetArray[i][j]])
            {
                updated[m_nCarpetArray[i][j]] = true;
                tile->Update(fDeltaTime);
            }

            if(sgn(tile->GetWidth() - m_pReader->GetTileWidth()) ||
                sgn(tile->GetHeight() - m_pReader->GetTileHeight()))
            {
                tile->RenderStretch(tx - m_fWorldLeft, ty - m_fWorldTop, m_pReader->GetTileWidth(), m_pReader->GetTileHeight());
            }
            else
            {
                tile->Render(tx - m_fWorldLeft, ty - m_fWorldTop);
            }
        }
    }
}

void OUMapView::RerenderBuildingLayer(float fDelta)
{
    static bool updated[65536];
    memset(updated, 0, sizeof(updated));

    for(int i = 0; i < m_pBuildings.size(); i++)
    {
        //if(m_pBuildings[i]->GetObjectType() != OUMIT_BUILDING)
        //{
        //    continue;
        //}

        if(!m_pBuildings[i]->IsInsideScreen(m_fWorldLeft, m_fWorldTop, m_rtMapScreen.GetWidth(), m_rtMapScreen.GetHeight()))
        {
            continue;
        }

        if(!updated[m_pBuildings[i]->GetResourceID()])
        {
            m_pBuildings[i]->Update(fDelta);
        }

        m_pBuildings[i]->SetZ(0.8f);
        m_pBuildings[i]->Render(m_fWorldLeft, m_fWorldTop);
    }
}

OUMapView* __pView = NULL;

bool rolecmp(OUMapItemObject* a, OUMapItemObject* b)
{
    OUMapItemRole* ba = (OUMapItemRole*)a;
    OUMapItemRole* bb = (OUMapItemRole*)b;

    float y1 = ba->GetY() - ba->GetX() / 2;
    float y2 = bb->GetY() - bb->GetX() / 2;

    if(sgn(y1 - y2) == 0)
    {
        return ba->GetY() < bb->GetY();
    }
    else
    if(sgn(y1 - y2) < 0) return true;
    else return false;
}

bool __pendcover(OUMapItemBuilding* b, OUMapItemRole* r)
{
    OUMapAStarCoor coorRole = __pView->GetCoorByTrueWorld(r->GetX(), r->GetY());
    if(b->Covered(coorRole.x, coorRole.y)) return true;
    else return false;
}

void OUMapView::RerenderRoleAndCover(float fDelta)
{
    __pView = this;

    static vector<OUMapItemObject*> pInScreen;
    pInScreen.clear();

    /** 每放一个建筑都放被这个建筑遮罩的人进去 */
    static set<int> used;
    used.clear();
    for(int i = 0; i < m_pBuildings.size(); i++)
    {
        if(!m_pBuildings[i]->IsInsideScreen(m_fWorldLeft, m_fWorldTop, m_rtMapScreen.GetWidth(), m_rtMapScreen.GetHeight()))
        {
            continue;
        }

        int start = pInScreen.size();

        /** 先探查自己 */
        if(used.find(-1) == used.end())
        {
            if(__pendcover(m_pBuildings[i], m_pMyRole))
            {
                used.insert(-1);
                pInScreen.push_back(m_pMyRole);
            }
        }

        /** 探查每个人物 */
        for(int j = 0; j < m_pRoles.size(); j++)
        {
            if(used.find(j) == used.end())
            {
                if(__pendcover(m_pBuildings[i], m_pRoles[j]))
                {
                    used.insert(j);
                    pInScreen.push_back(m_pRoles[j]);
                }
            }
        }

        /** 排序 */
        if(pInScreen.size() > start)
        {
            sort(pInScreen.begin() + start, pInScreen.end(), rolecmp);
        }

        pInScreen.push_back(m_pBuildings[i]);
    }
    int start = pInScreen.size();
    /** 先探查自己 */
    if(used.find(-1) == used.end())
    {
        pInScreen.push_back(m_pMyRole);
    }
    /** 探查每个人物 */
    for(int j = 0; j < m_pRoles.size(); j++)
    {
        if(used.find(j) == used.end())
        {
            used.insert(j);
            pInScreen.push_back(m_pRoles[j]);
        }
    }

    /** 排序 */
    if(pInScreen.size() > start)
    {
        sort(pInScreen.begin() + start, pInScreen.end(), rolecmp);
    }

    for(int i = 0; i < pInScreen.size(); i++)
    {
        if(pInScreen[i]->GetObjectType() == OUMIT_BUILDING || pInScreen[i]->GetObjectType() == OUMIT_TABLE)
        {
            OUMapItemBuilding* building = (OUMapItemBuilding*)pInScreen[i];
            building->RenderCover(m_fWorldLeft, m_fWorldTop);
        }
        else
        if(pInScreen[i]->GetObjectType() == OUMIT_ROLE)
        {
            OUMapItemRole* role = (OUMapItemRole*)pInScreen[i];
            role->Render(m_fWorldLeft, m_fWorldTop);
        }
    }
}

OUPOINT OUMapView::GetCenterOfCoor(int x, int y)
{
    float fy = (y + 1) * m_pReader->GetHalfTileHeight();
    float fx = (x + 1) * m_pReader->GetTileWidth() - m_pReader->GetHalfTileWidth();
    if(y % 2 == 1) fx += m_pReader->GetHalfTileWidth();

    OUPOINT pt;
    pt.m_fX = fx;
    pt.m_fY = fy;

    return pt;
}

void OUMapView::DrawSingleGrid(int sx, int sy, DWORD color)
{
    int ltx = sx, lty = sy;
    float m_nHalfTileWidth = m_pReader->GetHalfTileWidth();
    float m_nHalfTileHeight = m_pReader->GetHalfTileHeight();
    float m_nTileWidth = m_pReader->GetTileWidth();
    float m_nTileHeight = m_pReader->GetTileHeight();

    m_pHGE->Gfx_RenderLine(ltx + m_nHalfTileWidth, lty,
        ltx + m_nTileWidth, lty + m_nHalfTileHeight, color);
    m_pHGE->Gfx_RenderLine(ltx + m_nTileWidth, lty + m_nHalfTileHeight,
        ltx + m_nHalfTileWidth, lty + m_nTileHeight, color);
    m_pHGE->Gfx_RenderLine(ltx + m_nHalfTileWidth, lty + m_nTileHeight, 
        ltx, lty + m_nHalfTileHeight, color);
    m_pHGE->Gfx_RenderLine(ltx, lty + m_nHalfTileHeight, 
        ltx + m_nHalfTileWidth, lty, color);
}

OUMapAStarCoor OUMapView::GetCoorByTrueWorld(float fX, float fY)
{
    int nMouseWorldX = fX;
    int nMouseWorldY = fY;

    int m_dwHeight = m_nHeight;
    int m_dwWidth = m_nWidth;
    int m_nTileHeight = m_pReader->GetTileHeight();
    int m_nTileWidth = m_pReader->GetTileWidth();
    int m_nHalfTileHeight = m_pReader->GetHalfTileHeight();
    int m_nHalfTileWidth = m_pReader->GetHalfTileWidth();

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

    OUMapAStarCoor pt;
    pt.x = nCol;
    pt.y = nRow;

    return pt;
}
