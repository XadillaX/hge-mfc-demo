#include "StdAfx.h"
#include "OUMapView.h"

OUMapView::OUMapView(void)
{
    m_pHGE = hgeCreate(HGE_VERSION);
}

OUMapView::~OUMapView(void)
{
    m_pHGE->Release();
    m_pHGE = NULL;
}

void OUMapView::OnUpdate(float fDelta)
{
}

void OUMapView::Render()
{
}
