// OUNewMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUMapMakerEX.h"
#include "OUNewMapDlg.h"


// OUNewMapDlg �Ի���

IMPLEMENT_DYNAMIC(OUNewMapDlg, CDialog)

OUNewMapDlg::OUNewMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OUNewMapDlg::IDD, pParent)
    , m_szMapName("�ޱ���")
    , m_dwTileWidth(64)
    , m_dwTileHeight(32)
    , m_dwWidth(25)
    , m_dwHeight(64)
{
}

OUNewMapDlg::~OUNewMapDlg()
{
}

void OUNewMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_szMapName);
    DDV_MaxChars(pDX, m_szMapName, 60);
    DDX_Text(pDX, IDC_EDIT2, m_dwTileWidth);
    DDV_MinMaxUInt(pDX, m_dwTileWidth, 1, 1000);
    DDX_Text(pDX, IDC_EDIT3, m_dwTileHeight);
    DDV_MinMaxUInt(pDX, m_dwTileHeight, 1, 1000);
    DDX_Text(pDX, IDC_EDIT4, m_dwWidth);
    DDV_MinMaxUInt(pDX, m_dwWidth, 1, 1024);
    DDX_Text(pDX, IDC_EDIT5, m_dwHeight);
	DDV_MinMaxUInt(pDX, m_dwHeight, 1, 1024);
}


BEGIN_MESSAGE_MAP(OUNewMapDlg, CDialog)
END_MESSAGE_MAP()


// OUNewMapDlg ��Ϣ�������

void OUNewMapDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
    UpdateData(TRUE);

    CDialog::OnOK();
}

BOOL OUNewMapDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
