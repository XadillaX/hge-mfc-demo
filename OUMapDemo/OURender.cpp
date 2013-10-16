#include "StdAfx.h"
#include "OURender.h"
#include "OUAITestRole.h"

OURender::OURender(void) :
    m_pMap(NULL)
{
    srand(time(NULL));
    m_pHGE = hgeCreate(HGE_VERSION);

    ASSERT(m_pHGE != NULL);

    /** 载入测试地图 */
    m_pMap = new OUMapView();
    m_pMap->SetMap("map1.ogm");

    /** 人物 */
    m_pMyRole = new OUBaseRole();
    m_pMyRole->SetResource(1);
    m_pMyRole->SetMap(m_pMap);
    m_pMyRole->SetRoleCoor(4, 15);
    m_pMap->SetMyRole(m_pMyRole);

    /** 测试其它人物 */
    for(int i = 0; i < 50; i++)
    {
        OUAITestRole* test = new OUAITestRole();
        test->SetResource(2);
        test->SetMap(m_pMap);
        test->RandomPos();

        m_pMap->AddOtherRole(test);
        printf("%d\n", i);
    }
}

OURender::~OURender(void)
{
    SAFEDEL(m_pMap);
    SAFEDEL(m_pMyRole);

    for(int i = 0; i < m_pRoles.size(); i++)
    {
        SAFEDEL(m_pRoles[i]);
    }
    m_pRoles.clear();

    m_pHGE->Release();
    m_pHGE = NULL;

    OUMapResourceRole::ReleaseAll();
}

void OURender::Update(float fDelta)
{
    float mx, my;
    bool over = m_pHGE->Input_IsMouseOver();
    if(over)
    {
        m_pHGE->Input_GetMousePos(&mx, &my);
    }

    if(m_pMap)
    {
        if(m_pHGE->Input_GetKeyState(HGEK_LEFT))
        {
            m_pMap->MoveBy(fDelta * -500, 0);
        }
        if(m_pHGE->Input_GetKeyState(HGEK_RIGHT))
        {
            m_pMap->MoveBy(fDelta * 500, 0);
        }
        if(m_pHGE->Input_GetKeyState(HGEK_UP))
        {
            m_pMap->MoveBy(0, fDelta * -500);
        }
        if(m_pHGE->Input_GetKeyState(HGEK_DOWN))
        {
            m_pMap->MoveBy(0, fDelta * 500);
        }

        if(over)
        {
            hgeRect mapRect = m_pMap->GetMapRect();
            m_pMap->Update(mx - mapRect.x1, my - mapRect.y1, fDelta);
        }
        m_pMap->RebuildTarget(fDelta);
    }
}

void OURender::Render()
{
    if(m_pMap)
    {
        hgeRect mapRect = m_pMap->GetMapRect();
        m_pMap->GetSprite()->Render(mapRect.x1, mapRect.y1);
    }
}
