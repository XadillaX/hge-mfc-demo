// OUNewMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUMapMaker.h"
#include "OUNewMapDlg.h"


// OUNewMapDlg �Ի���

IMPLEMENT_DYNAMIC(OUNewMapDlg, CDialog)

OUNewMapDlg::OUNewMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OUNewMapDlg::IDD, pParent)
    , m_szMapName(_T(""))
    , m_dwWidth(0)
    , m_dwHeight(0)
{

}

OUNewMapDlg::~OUNewMapDlg()
{
}

void OUNewMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_NMAP_MAPNAME, m_szMapName);
    DDV_MaxChars(pDX, m_szMapName, 32);
    DDX_Text(pDX, IDC_NMAP_WIDTH, m_dwWidth);
    DDV_MinMaxUInt(pDX, m_dwWidth, 1, 1000);
    DDX_Text(pDX, IDC_NMAP_HEIGHT, m_dwHeight);
	DDV_MinMaxUInt(pDX, m_dwHeight, 1, 1000);
}


BEGIN_MESSAGE_MAP(OUNewMapDlg, CDialog)
END_MESSAGE_MAP()


// OUNewMapDlg ��Ϣ�������

BOOL OUNewMapDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_dwWidth = m_dwHeight = 25;
    m_szMapName = "δ����";
    UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
