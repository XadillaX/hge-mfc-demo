// OUAssetsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUMapMakerEX.h"
#include "OUAssetsDlg.h"


// OUAssetsDlg �Ի���

IMPLEMENT_DYNAMIC(OUAssetsDlg, CDialog)

OUAssetsDlg::OUAssetsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OUAssetsDlg::IDD, pParent)
{

}

OUAssetsDlg::~OUAssetsDlg()
{
}

void OUAssetsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TABASSETSTAB, m_AssetsTab);
}


BEGIN_MESSAGE_MAP(OUAssetsDlg, CDialog)
    ON_WM_SIZE()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TABASSETSTAB, &OUAssetsDlg::OnTcnSelchangeTabassetstab)
END_MESSAGE_MAP()


// OUAssetsDlg ��Ϣ�������

void OUAssetsDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    /** Tab���С */
    if(m_AssetsTab.GetSafeHwnd())
    {
        m_AssetsTab.MoveWindow(10, 10, cx - 20, cy - 20, 1);

        /** Tab���ڴ��ڴ�С */
        CRect rect;
        m_AssetsTab.GetClientRect(&rect);
        rect.top += 30;
        rect.bottom -= 8;
        rect.left += 8;
        rect.right -= 8;

        for(int i = 0; i < OUAT_MAX; i++)
        {
            m_AssetsTabDlg[i].MoveWindow(&rect, 1);
        }
    }
}

BOOL OUAssetsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    /** �Ϸ���׺ */
    vector<CString> exts;
    //exts.push_back(".oga");
    exts.push_back(".png");
    exts.push_back(".jpg");
    exts.push_back(".bmp");

    /** ��ʼ��Tab */
    TCITEM item;

    item.mask = TCIF_TEXT;
    item.pszText = "�ر��";
    m_AssetsTab.InsertItem(0, &item);
    m_AssetsTabDlg[0].InitInfo(OUAT_BOTTOM, "tile\\road\\", "tile%d", exts);
    m_AssetsTabDlg[0].Create(IDD_ASSETSTABDLG, &m_AssetsTab);
    m_AssetsTabDlg[0].ShowWindow(SW_SHOW);
    m_AssetsTab.SetCurSel(0);

    item.pszText = "��̺��";
    m_AssetsTab.InsertItem(1, &item);
    m_AssetsTabDlg[1].InitInfo(OUAT_CARPET, "tile\\carpet\\", "carpet%d", exts);
    m_AssetsTabDlg[1].Create(IDD_ASSETSTABDLG, &m_AssetsTab);
    m_AssetsTabDlg[1].ShowWindow(SW_HIDE);

    item.pszText = "������";
    m_AssetsTab.InsertItem(2, &item);
    m_AssetsTabDlg[2].InitInfo(OUAT_BUILDING, "tile\\building\\", "building%d", exts);
    m_AssetsTabDlg[2].Create(IDD_ASSETSTABDLG, &m_AssetsTab);
    m_AssetsTabDlg[2].ShowWindow(SW_HIDE);

    //item.pszText = "������";
    //m_AssetsTab.InsertItem(1, &item);
    //m_AssetsTabDlg[1].InitInfo(OUAT_BOTTOM, "tile\\building\\", "wall%d", exts);
    //m_AssetsTabDlg[1].Create(IDD_ASSETSTABDLG, &m_AssetsTab);
    //m_AssetsTabDlg[1].ShowWindow(SW_HIDE);

    //item.pszText = "���ֲ�";
    //m_AssetsTab.InsertItem(2, &item);
    //m_AssetsTabDlg[2].InitInfo(OUAT_BOTTOM, "tile\\cover\\", "cover%d", exts);
    //m_AssetsTabDlg[2].Create(IDD_ASSETSTABDLG, &m_AssetsTab);
    //m_AssetsTabDlg[2].ShowWindow(SW_HIDE);

    //item.pszText = "��ղ�";
    //m_AssetsTab.InsertItem(3, &item);
    //m_AssetsTabDlg[3].InitInfo(OUAT_BOTTOM, "tile\\sky\\", "sky%d", exts);
    //m_AssetsTabDlg[3].Create(IDD_ASSETSTABDLG, &m_AssetsTab);
    //m_AssetsTabDlg[3].ShowWindow(SW_HIDE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void OUAssetsDlg::OnTcnSelchangeTabassetstab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    /** �ı��б�� */
    static int nCurSel = 0;
    m_AssetsTabDlg[nCurSel].ShowWindow(SW_HIDE);
    nCurSel = m_AssetsTab.GetCurSel();
    m_AssetsTabDlg[nCurSel].ShowWindow(SW_SHOW);

    *pResult = 0;
}
