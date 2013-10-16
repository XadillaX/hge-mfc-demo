#include "StdAfx.h"
#include "OUMapItemRole.h"

OUMapItemRole::OUMapItemRole(void) :
    m_emState(OURAS_STANDING),
    m_emFaceDir(OURFD_DOWN),
    m_nOrder(0)
{
    m_emClassType = OUMIT_ROLE;

    Release();
}

OUMapItemRole::~OUMapItemRole(void)
{
    Release();
}

void OUMapItemRole::Release()
{
    memset(m_pStanding, 0, sizeof(m_pStanding));
    memset(m_pRun, 0, sizeof(m_pRun));
    m_pSprite = NULL;
    
    m_emState = OURAS_STANDING;
    m_emFaceDir = OURFD_DOWN;
}

bool OUMapItemRole::SetResource(DWORD dwID)
{
    Release();

    OUMapResourceRole* res = new OUMapResourceRole();
    if(!res->Load(dwID))
    {
        SAFEDEL(res);
        return false;
    }

    memcpy(m_pStanding, res->GetAnimationArray(OURAS_STANDING), sizeof(m_pStanding));
    memcpy(m_pRun, res->GetAnimationArray(OURAS_RUN), sizeof(m_pRun));

    m_pSprite = (OUMapResourceObject*)res;

    SetStateAndFaceDir(OURAS_RUN, OURFD_DOWN);

    return true;
}

hgeAnimation* OUMapItemRole::GetCurAnimation()
{
    hgeAnimation* cur;
    switch(m_emState)
    {
    case OURAS_STANDING:
        {
            cur = m_pStanding[m_emFaceDir];
            break;
        }

    case OURAS_RUN:
        {
            cur = m_pRun[m_emFaceDir];
            break;
        }

    default: break;
    }

    return cur;
}

void OUMapItemRole::Update(float fDelta)
{
    hgeAnimation* cur = GetCurAnimation();
    if(cur == NULL) return;
    if(!cur->IsPlaying()) cur->Play();

    cur->Update(fDelta);
}

void OUMapItemRole::Render(float fStartX, float fStartY)
{
    hgeAnimation* cur = GetCurAnimation();
    if(cur == NULL) return;

    cur->Render(m_fWorldX - fStartX, m_fWorldY - fStartY);
}

void OUMapItemRole::SetState(OUMAPROLE_ACT_STATE state)
{
    if(state == m_emState) return;

    hgeAnimation* cur = GetCurAnimation();
    if(cur != NULL)
    {
        cur->Stop();
    }

    m_emState = state;
    cur = GetCurAnimation();
    if(!cur)
    {
        this->m_fWidth = this->m_fHeight = 0;
    }

    cur->Play();
    m_fWidth = cur->GetWidth();
    m_fHeight = cur->GetHeight();
}

void OUMapItemRole::SetFaceDir(OUMAPROLE_FACE_DIR dir)
{
    if(dir == m_emFaceDir) return;

    hgeAnimation* cur = GetCurAnimation();
    if(cur != NULL)
    {
        cur->Stop();
    }

    m_emFaceDir = dir;
    cur = GetCurAnimation();
    if(!cur)
    {
        this->m_fWidth = this->m_fHeight = 0;
    }

    cur->Play();
    m_fWidth = cur->GetWidth();
    m_fHeight = cur->GetHeight();
}

void OUMapItemRole::SetStateAndFaceDir(OUMAPROLE_ACT_STATE state, OUMAPROLE_FACE_DIR dir)
{
    if(state == m_emState && dir == m_emFaceDir) return;

    hgeAnimation* cur = GetCurAnimation();
    if(cur != NULL)
    {
        cur->Stop();
    }

    m_emFaceDir = dir;
    m_emState = state;
    cur = GetCurAnimation();
    if(!cur)
    {
        this->m_fWidth = this->m_fHeight = 0;
    }

    cur->Play();
    m_fWidth = cur->GetWidth();
    m_fHeight = cur->GetHeight();
}

bool OUMapItemRole::IsInsideScreen(float fStartX, float fStartY, float fScreenWidth, float fScreenHeight)
{
    hgeAnimation* cur = GetCurAnimation();
    if(cur == NULL) return false;

    float hx, hy;
    cur->GetHotSpot(&hx, &hy);

    hgeRect rect1(fStartX, fStartY, fStartX + fScreenWidth, fStartY + fScreenHeight);
    hgeRect rect2(m_fWorldX - hx, m_fWorldY - hy, m_fWorldX + m_fWidth - hx, m_fWorldY + m_fHeight - hy);

    return rect1.Intersect(&rect2);
}
