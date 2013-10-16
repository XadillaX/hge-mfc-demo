// OUAssetsTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OUMapMakerEX.h"
#include "OUAssetsTabDlg.h"

//tagOUMAP_UPPER_CELL* tagOUMAP_UPPER_CELL::m_pFocused = NULL;

// OUAssetsTabDlg 对话框

IMPLEMENT_DYNAMIC(OUAssetsTabDlg, CDialog)

OUAssetsTabDlg::OUAssetsTabDlg(CWnd* pParent /*=NULL*/) :
	CDialog(OUAssetsTabDlg::IDD, pParent),
    m_bInited(false)
{
}

OUAssetsTabDlg::~OUAssetsTabDlg()
{
}

void OUAssetsTabDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ASSETSLIST, m_AssetsList);
    DDX_Control(pDX, IDC_COUNT, m_AssetsCount);
}


BEGIN_MESSAGE_MAP(OUAssetsTabDlg, CDialog)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_NOTIFY(HDN_ITEMCHANGED, 0, &OUAssetsTabDlg::OnHdnItemchangedAssetslist)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_ASSETSLIST, &OUAssetsTabDlg::OnLvnItemchangedAssetslist)
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// OUAssetsTabDlg 消息处理程序

void OUAssetsTabDlg::InitInfo(OUASSETSTAB_LAYER emLayer, const char* szPath, const char* szFormat, vector<CString>& arrExts)
{
    if(m_bInited) return;

    m_emLayer = emLayer;
    m_szPath = szPath;
    m_szFormat = szFormat;

    for(int i = 0; i < arrExts.size(); i++)
    {
        m_arrExts.push_back(arrExts[i].GetBuffer());
    }

    if(OUAT_BUILDING == this->m_emLayer)
    {
        m_arrExts.push_back(".INI");
    }

    m_bInited = true;
}

void OUAssetsTabDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if(m_AssetsList.GetSafeHwnd())
    {
        //m_AssetsList.MoveWindow(10, 10, cx - 20, cy - 20, 1);
        m_AssetsList.MoveWindow(0, 0, cx, cy - 20, 1);
        //m_AssetsList.RedrawItems(0, m_AssetsList.GetItemCount() - 1);
        m_AssetsList.Arrange(LVA_ALIGNTOP);
        m_AssetsCount.MoveWindow(5, cy - 15, cx, 20, 1);
    }
}

CBitmap* OUAssetsTabDlg::ScaleBitmap(CImage* pImg, int maxwidth, int maxheight)
{
    /** 计算新大小和位置 */
    int nWidth = pImg->GetWidth();
    int nHeight = pImg->GetHeight();
    int nNewWidth = nWidth, nNewHeight = nHeight;
    int x, y;
    if(nWidth > maxwidth || nHeight > maxheight)
    {
        int nWidth1, nWidth2;
        int nHeight1, nHeight2;

        nWidth1 = maxwidth, nHeight1 = (float)(((float)maxwidth / (float)(nWidth)) * (float)nHeight);
        nHeight2 = maxheight, nWidth2 = (float)(((float)maxheight / (float)(nHeight)) * (float)nWidth);

        if(nHeight1 > maxheight) nNewWidth = nWidth2, nNewHeight = nHeight2;
        else
        if(nWidth2 > maxwidth) nNewWidth = nWidth1, nNewHeight = nHeight1;
        else
        {
            nNewWidth = max(nWidth1, nWidth2);
            nNewHeight = max(nHeight1, nHeight2);
        }
    }
    x = (maxwidth - nNewWidth) >> 1;
    y = (maxheight - nNewHeight) >> 1;

    CBitmap* pImgCpy = new CBitmap();
    pImgCpy->DeleteObject();
    pImgCpy->Attach(pImg->operator HBITMAP());

    CDC* pMDC1 = new CDC();
    CDC* pMDC2 = new CDC();
    CBitmap* pOldBt1;
    CBitmap* pOldBt2;
    CBitmap* pBmPtr = new CBitmap();
    CClientDC dc(this);

    pMDC2->CreateCompatibleDC(&dc);
    pOldBt2 = pMDC2->SelectObject(pImgCpy);

    pMDC1->CreateCompatibleDC(&dc);
    pBmPtr->CreateCompatibleBitmap(&dc, maxwidth, maxheight);
    pOldBt1 = pMDC1->SelectObject(pBmPtr);

    BITMAP bm;
    GetObject(*pImgCpy, sizeof(bm), &bm);
    pMDC1->StretchBlt(x, y, nNewWidth, nNewHeight, pMDC2, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    pMDC1->SelectObject(pOldBt1);
    pMDC1->DeleteDC();
    pMDC2->SelectObject(pOldBt2);
    pMDC2->DeleteDC();

    return pBmPtr;
}

BOOL OUAssetsTabDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_AssetsImages.Create(64, 64, ILC_COLOR32 /** | ILC_MASK */, 0, 4);
    m_AssetsList.SetImageList(&m_AssetsImages, LVSIL_NORMAL);
    m_AssetsList.SetBkColor(0x000000);
    m_AssetsList.SetTextBkColor(0x000000);
    m_AssetsList.SetTextColor(0xffffff);
    //m_AssetsList.SetExtendedStyle(m_AssetsList.GetExtendedStyle());

    /** 载入资源 */
    vector<string> filenames;
    static char name[512];
    OUFileList(m_szPath.GetBuffer(), m_arrExts, filenames);
    OUStringFunc func1, func2;

    /** 寻找每个文件的合法性 */
    for(int i = 0; i < filenames.size(); i++)
    {
        bool flag = false;
        func1 = filenames[i];
        func1.ToUppercase();

        /** 是否桌子 */
        if(m_emLayer == OUASSETSTAB_LAYER::OUAT_BUILDING)
        {
            for(int j = 1; j <= 10; j++)
            {
                sprintf(name, "table%d.ini", j);
                func2.SetString(string(name));
                func2.ToUppercase();

                if(func1.GetString() == func2.GetString())
                {
                    flag = true;
                    OUASSETSITEM item;
                    item.filename = func2.GetString();
                    item.id = 2147483647 - j + 1;

                    /** 载入图片资源 */
                    OUIniFile ini(string("tile\\building\\" + func2.GetString()).c_str());
                    CString path = "tile\\table\\";
                    item.img = new CImage();
                    item.img->Load(path + ini.GetString("table", "img").c_str());
                    item.bitmap = ScaleBitmap(item.img, 64, 64);

                    /** 加入list */
                    item.imgid = m_AssetsImages.Add(item.bitmap, 0xff00ff);

                    InsertToList(item);
                    m_Items[item.id] = item;

                    break;
                }
            }
            if(flag) continue;
        }

        /** ID从1到MAXID枚举 */
        for(int j = 1; j <= OUASSETS_MAXID; j++)
        {
            sprintf(name, m_szFormat, j);

            /** 贴上后缀 */
            for(int k = 0; k < m_arrExts.size(); k++)
            {
                func2.SetString(name + m_arrExts[k]);
                func2.ToUppercase();

                /** 若文件名合法 */
                if(func1.GetString() == func2.GetString())
                {
                    flag = true;
                    if(m_Items.find(j) == m_Items.end())
                    {
                        OUASSETSITEM item;
                        item.filename = func1.GetString();
                        item.id = j;

                        /** 载入图片资源 */
                        item.img = new CImage();
                        item.img->Load(m_szPath + func1.GetString().c_str());
                        item.bitmap = ScaleBitmap(item.img, 64, 64);

                        /** 加入ImageList */
                        item.imgid = m_AssetsImages.Add(item.bitmap, 0xff00ff);

                        /** 存入Map */
                        InsertToList(item);
                        m_Items[item.id] = item;
                    }
                    
                    break;
                }
            }
            if(flag) break;
        }
    }

    /** 资源数 */
    sprintf(name, "资源数：%d", m_Items.size());
    m_AssetsCount.SetWindowText(name);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void OUAssetsTabDlg::InsertToList(OUASSETSITEM& item)
{
    char id[15];
    sprintf(id, "%.5d", item.id);
    m_AssetsList.InsertItem(m_AssetsList.GetItemCount() - 1, id, item.imgid);

    //m_AssetsList.SetImageList
}

void OUAssetsTabDlg::OnDestroy()
{
    CDialog::OnDestroy();

    for(map<int, OUASSETSITEM>::iterator it = m_Items.begin(); it != m_Items.end(); it++)
    {
        it->second.bitmap->DeleteObject();
        delete it->second.bitmap;
        it->second.img->Destroy();
        delete it->second.img;
    }
    m_Items.clear();
}

void OUAssetsTabDlg::OnHdnItemchangedAssetslist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    *pResult = 0;
}

void OUAssetsTabDlg::OnLvnItemchangedAssetslist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->uChanged == LVIF_STATE)
    {
        if(pNMListView->uNewState)
        {
            int nItem = pNMListView->iItem;
            
            CString str = m_AssetsList.GetItemText(nItem, 0);
            int key = atoi(str);

            if(m_Items.find(key) == m_Items.end()) return;
            OUASSETSITEM item = m_Items[key];
            OM_SELECTTILE_PARAM* lpParam = new OM_SELECTTILE_PARAM();
            lpParam->m_emResType = m_emLayer;
            lpParam->m_nID = item.id;
            g_pView->SendMessage(OM_SELECTTILE, lpParam);

            TRACE0("选择改变且有选中的行\n");
        }
        else
        {
            OM_SELECTTILE_PARAM* lpParam = new OM_SELECTTILE_PARAM();
            lpParam->m_emResType = m_emLayer;
            lpParam->m_nID = 0;
            g_pView->SendMessage(OM_SELECTTILE, lpParam);

            TRACE0("选择改变且没有选中的行\n");
        }
    }
    else TRACE0("行改变(CListCtrl::SetItem)\n");

    *pResult = 0;
}

bool OUAssetsTabDlg::m_bFirstShow = true;
void OUAssetsTabDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    if(m_bFirstShow)
    {
        m_bFirstShow = false;
        return;
    }

    // TODO: 在此处添加消息处理程序代码
    if(bShow)
    {
        /** 找到被选中原件 */
        POSITION pos = m_AssetsList.GetFirstSelectedItemPosition();
        if(pos == NULL)
        {
            OM_SELECTTILE_PARAM* lpParam = new OM_SELECTTILE_PARAM();
            lpParam->m_emResType = m_emLayer;
            lpParam->m_nID = 0;
            g_pView->SendMessage(OM_SELECTTILE, lpParam);
        }
        else
        {
            int nItem = m_AssetsList.GetNextSelectedItem(pos);
            CString str = m_AssetsList.GetItemText(nItem, 0);
            int key = atoi(str);

            if(m_Items.find(key) == m_Items.end()) return;
            OUASSETSITEM item = m_Items[key];
            OM_SELECTTILE_PARAM* lpParam = new OM_SELECTTILE_PARAM();
            lpParam->m_emResType = m_emLayer;
            lpParam->m_nID = item.id;
            g_pView->SendMessage(OM_SELECTTILE, lpParam);
        }
    }
}
