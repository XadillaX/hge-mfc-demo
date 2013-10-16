#include "StdAfx.h"
#include "OUMapView.h"
#include "OUBaseRole.h"

OUBaseRole::OUBaseRole(void) :
    m_pMap(NULL),
    m_fTargetX(-1.0f),
    m_fTargetY(-1.0f),
    m_fNextX(-1.0f),
    m_fNextY(-1.0f)
{
}

OUBaseRole::~OUBaseRole(void)
{
}

void OUBaseRole::SetMap(OUMap* map)
{
    m_pMap = map;
    m_pAStar = map->GetAStar();
}

void OUBaseRole::SetRoleCoor(int x, int y)
{
    VERIFY(m_pMap != NULL);

    OUPOINT pt = m_pMap->GetCenterOfCoor(x, y);
    m_fWorldX = pt.m_fX;
    m_fWorldY = pt.m_fY;
}

void OUBaseRole::Goto(float truex, float truey)
{
    if(sgn(truex - m_fWorldX) == 0 && sgn(truey - m_fWorldY) == 0)
    {
        m_fTargetX = -1.0f;
        m_fTargetY = -1.0f;
    }
    else
    {
        /** 获得当前鼠标位置 */
        OUMapAStarCoor coor = m_pMap->GetCoorByTrueWorld(truex, truey);
        if(coor.x < 0 || coor.y < 0 ||
            coor.x >= m_pMap->GetWidth() || coor.y >= m_pMap->GetHeight())
        {
            return;
        }

        /** 若之前寻径了 */
        if(m_fTargetX >= 0 && m_fTargetY >= 0)
        {
            OUMapAStarCoor tarcoor = m_pMap->GetCoorByTrueWorld(m_fTargetX, m_fTargetY);
            if(tarcoor == coor)
            {
                return;
            }
        }

        /** 开始寻径 */
        OUMapAStarCoor curcoor = m_pMap->GetCoorByTrueWorld(m_fWorldX, m_fWorldY);
        /** 如果是当前坐标 */
        if(curcoor == coor)
        {
            OUPOINT center = m_pMap->GetCenterOfCoor(curcoor.x, curcoor.y);
            m_fTargetX = center.m_fX;
            m_fTargetY = center.m_fY;
            m_fNextX = m_fTargetX;
            m_fNextY = m_fTargetY;
            return;
        }

        if(!m_pAStar->CheckCoor(coor.x, coor.y)) return;
        bool finded;
        if(m_fNextX < 0 && m_fNextY < 0)
        {
            finded = m_pAStar->FindPath(curcoor.x, curcoor.y, coor.x, coor.y);
        }
        else
        {
            OUMapAStarCoor oldnext = m_pMap->GetCoorByTrueWorld(m_fNextX, m_fNextY);
            finded = m_pAStar->FindPath(oldnext.x, oldnext.y, coor.x, coor.y);
        }
        if(!finded) return;

        /** 若是已有寻径 */
        if(m_fNextX >= 0 && m_fNextY >= 0)
        {
            OUPOINT truetar = m_pMap->GetCenterOfCoor(coor.x, coor.y);
            m_fTargetX = truetar.m_fX;
            m_fTargetY = truetar.m_fY;

            return;
        }

        OUMapAStarCoor next;
        bool nexted = m_pAStar->GetNextCoor(&next);
        if(!nexted) return;

        
        OUPOINT truenext = m_pMap->GetCenterOfCoor(next.x, next.y);
        if(curcoor == next)
        //if(next == oldnext)
        {
            nexted = m_pAStar->GetNextCoor(&next);
            if(!nexted)
            {
                OUPOINT truetar = m_pMap->GetCenterOfCoor(coor.x, coor.y);

                m_fTargetX = truetar.m_fX;
                m_fTargetY = truetar.m_fY;
                return;
            }
            truenext = m_pMap->GetCenterOfCoor(next.x, next.y);
        }

        OUPOINT truetar = m_pMap->GetCenterOfCoor(coor.x, coor.y);
        m_fNextX = truenext.m_fX;
        m_fNextY = truenext.m_fY;
        m_fTargetX = truetar.m_fX;
        m_fTargetY = truetar.m_fY;
    }
}

void OUBaseRole::Update(float fDelta)
{
    if(m_fTargetX >= 0 && m_fTargetY >= 0)
    {
        UpdateMove(fDelta);
    }
    else
    {
        SetState(OURAS_STANDING);
    }

    OUMapItemRole::Update(fDelta);
}

void OUBaseRole::UpdateMove(float fDelta)
{
    float dx, dy;
    dx = m_fNextX - m_fWorldX;
    dy = m_fNextY - m_fWorldY;

    /** 右下方 */
    if(sgn(dx) > 0 && sgn(dy) > 0)
    {
        float dummyx, dummyy;
        dummyx = m_fWorldX + fDelta * OUROLE_DEFAULT_SPEEDX;
        dummyy = m_fWorldY + fDelta * OUROLE_DEFAULT_SPEEDY;

        SetStateAndFaceDir(OURAS_RUN, OURFD_DOWNRIGHT);

        /** 走过头了 */
        if(sgn(dummyx - m_fNextX) > 0 || sgn(dummyy - m_fNextY) > 0)
        {
            //float dummyx1, dummyy1;
            //dummyx1 = ((m_fNextY - m_fWorldY) / OUROLE_DEFAULT_SPEEDY) * OUROLE_DEFAULT_SPEEDX + m_fWorldX;
            //dummyy1 = ((m_fNextX - m_fWorldX) / OUROLE_DEFAULT_SPEEDX) * OUROLE_DEFAULT_SPEEDY + m_fWorldY;

            //if(sgn(dummyx - m_fNextX) > 0)
            //{
            //    MoveToCoorThisFrame(m_fNextX, dummyy1);
            //}
            //else
            //{
            //    MoveToCoorThisFrame(dummyx1, m_fNextY);
            //}
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else MoveToCoorThisFrame(dummyx, dummyy);
    }
    /** 左上方 */
    else
    if(sgn(dx) < 0 && sgn(dy) < 0)
    {
        float dummyx, dummyy;
        dummyx = m_fWorldX - fDelta * OUROLE_DEFAULT_SPEEDX;
        dummyy = m_fWorldY - fDelta * OUROLE_DEFAULT_SPEEDY;

        SetStateAndFaceDir(OURAS_RUN, OURFD_UPLEFT);

        /** 走过头了 */
        if(sgn(dummyx - m_fNextX) < 0 || sgn(dummyy - m_fNextY) < 0)
        {
            //float dummyx1, dummyy1;
            //dummyx1 = -((m_fNextY - m_fWorldY) / OUROLE_DEFAULT_SPEEDY) * OUROLE_DEFAULT_SPEEDX + m_fWorldX;
            //dummyy1 = -((m_fNextX - m_fWorldX) / OUROLE_DEFAULT_SPEEDX) * OUROLE_DEFAULT_SPEEDY + m_fWorldY;

            //if(sgn(dummyx - m_fNextX) < 0)
            //{
            //    MoveToCoorThisFrame(m_fNextX, dummyy1);
            //}
            //else
            //{
            //    MoveToCoorThisFrame(dummyx1, m_fNextY);
            //}
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else MoveToCoorThisFrame(dummyx, dummyy);
    }
    /** 右上方 */
    else
    if(sgn(dx) > 0 && sgn(dy) < 0)
    {
        float dummyx, dummyy;
        dummyx = m_fWorldX + fDelta * OUROLE_DEFAULT_SPEEDX;
        dummyy = m_fWorldY - fDelta * OUROLE_DEFAULT_SPEEDY;

        SetStateAndFaceDir(OURAS_RUN, OURFD_UPRIGHT);

        /** 走过头了 */
        if(sgn(dummyx - m_fNextX) > 0 || sgn(dummyy - m_fNextY) < 0)
        {
            //float dummyx1, dummyy1;
            //dummyx1 = ((m_fNextY - m_fWorldY) / OUROLE_DEFAULT_SPEEDY) * OUROLE_DEFAULT_SPEEDX + m_fWorldX;
            //dummyy1 = -((m_fNextX - m_fWorldX) / OUROLE_DEFAULT_SPEEDX) * OUROLE_DEFAULT_SPEEDY + m_fWorldY;

            //if(sgn(dummyx - m_fNextX) > 0)
            //{
            //    MoveToCoorThisFrame(m_fNextX, dummyy1);
            //}
            //else
            //{
            //    MoveToCoorThisFrame(dummyx1, m_fNextY);
            //}
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else MoveToCoorThisFrame(dummyx, dummyy);
    }
    /** 左下方 */
    else
    if(sgn(dx) < 0 && sgn(dy) > 0)
    {
        float dummyx, dummyy;
        dummyx = m_fWorldX - fDelta * OUROLE_DEFAULT_SPEEDX;
        dummyy = m_fWorldY + fDelta * OUROLE_DEFAULT_SPEEDY;

        SetStateAndFaceDir(OURAS_RUN, OURFD_DOWNLEFT);

        /** 走过头了 */
        if(sgn(dummyx - m_fNextX) < 0 || sgn(dummyy - m_fNextY) > 0)
        {
            //float dummyx1, dummyy1;
            //dummyx1 = -((m_fNextY - m_fWorldY) / OUROLE_DEFAULT_SPEEDY) * OUROLE_DEFAULT_SPEEDX + m_fWorldX;
            //dummyy1 = ((m_fNextX - m_fWorldX) / OUROLE_DEFAULT_SPEEDX) * OUROLE_DEFAULT_SPEEDY + m_fWorldY;

            //if(sgn(dummyx - m_fNextX) < 0)
            //{
            //    MoveToCoorThisFrame(m_fNextX, dummyy1);
            //}
            //else
            //{
            //    MoveToCoorThisFrame(dummyx1, m_fNextY);
            //}
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else MoveToCoorThisFrame(dummyx, dummyy);
    }
    else
    /** 左 */
    if(sgn(dx) < 0)
    {
        SetStateAndFaceDir(OURAS_RUN, OURFD_LEFT);

        float dummyx = m_fWorldX - fDelta * OUROLE_DEFAULT_SPEEDX;
        if(sgn(dummyx - m_fNextX) < 0)
        {
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else
        {
            MoveToCoorThisFrame(dummyx, m_fNextY);
        }
    }
    else
    /** 右 */
    if(sgn(dx) > 0)
    {
        SetStateAndFaceDir(OURAS_RUN, OURFD_RIGHT);

        float dummyx = m_fWorldX + fDelta * OUROLE_DEFAULT_SPEEDX;
        if(sgn(dummyx - m_fNextX) > 0)
        {
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else
        {
            MoveToCoorThisFrame(dummyx, m_fNextY);
        }
    }
    else
    /** 上 */
    if(sgn(dy) < 0)
    {
        SetStateAndFaceDir(OURAS_RUN, OURFD_UP);

        float dummyy = m_fWorldY - fDelta * OUROLE_DEFAULT_SPEEDY;
        if(sgn(dummyy - m_fNextY) < 0)
        {
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else
        {
            MoveToCoorThisFrame(m_fNextX, dummyy);
        }
    }
    else
    /** 下 */
    if(sgn(dy) > 0)
    {
        SetStateAndFaceDir(OURAS_RUN, OURFD_DOWN);

        float dummyy = m_fWorldY + fDelta * OUROLE_DEFAULT_SPEEDY;
        if(sgn(dummyy - m_fNextY) > 0)
        {
            MoveToCoorThisFrame(m_fNextX, m_fNextY);
        }
        else
        {
            MoveToCoorThisFrame(m_fNextX, dummyy);
        }
    }
    else
    {
        if(m_fTargetX == m_fNextX && m_fTargetY == m_fNextY)
        {
            m_fTargetX = m_fTargetY = -1.0f;
            m_fNextX = m_fNextY = -1.0f;
            SetState(OURAS_STANDING);
        }
        else
        {
            OUMapAStarCoor coor;
            bool next = m_pAStar->GetNextCoor(&coor);
            if(!next)
            {
                m_fTargetX = m_fTargetY = -1.0f;
                m_fNextX = m_fNextY = -1.0f;
                SetState(OURAS_STANDING);
                return;
            }

            OUPOINT center = m_pMap->GetCenterOfCoor(coor.x, coor.y);
            m_fNextX = center.m_fX;
            m_fNextY = center.m_fY;
        }
        //m_fTargetX = m_fTargetY = -1.0f;
        //SetState(OURAS_STANDING);
    }
}

void OUBaseRole::MoveToCoorThisFrame(float x, float y)
{
    m_fWorldX = x;
    m_fWorldY = y;
}
