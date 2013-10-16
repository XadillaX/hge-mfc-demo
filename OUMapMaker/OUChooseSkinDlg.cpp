// OUChooseSkinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OUMapMaker.h"
#include "OUChooseSkinDlg.h"


// OUChooseSkinDlg 对话框

IMPLEMENT_DYNAMIC(OUChooseSkinDlg, CDialog)

OUChooseSkinDlg::OUChooseSkinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OUChooseSkinDlg::IDD, pParent),
    m_szCurSelected("")
{

}

OUChooseSkinDlg::~OUChooseSkinDlg()
{
}

void OUChooseSkinDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(OUChooseSkinDlg, CDialog)
    ON_WM_CREATE()
    ON_LBN_SELCHANGE(IDC_LIST1, &OUChooseSkinDlg::OnLbnSelchangeList1)
    ON_LBN_DBLCLK(IDC_LIST1, &OUChooseSkinDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// OUChooseSkinDlg 消息处理程序

int OUChooseSkinDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    

    return 0;
}

BOOL OUChooseSkinDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    /** 寻找文件名 */
    vector<string> filenames;
    vector<string> exts;
    exts.push_back(".ssk");
    exts.push_back(".spp");
    ::OUFileList(string("skin\\"), exts, filenames);

    OUStringFunc orgSkin(g_szCurSkinName);
    orgSkin.ToUppercase();
    for(int i = 0; i < filenames.size(); i++)
    {
        m_ListBox.AddString(filenames[i].c_str());
        OUStringFunc thisName = filenames[i];
        thisName.ToUppercase();

        if(thisName.GetString() == orgSkin.GetString())
        {
            m_ListBox.SetCurSel(i);
            m_szCurSelected = filenames[i];
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

string OUChooseSkinDlg::GetSelectedSkin()
{
    return m_szCurSelected;
}

void OUChooseSkinDlg::OnLbnSelchangeList1()
{
    // TODO: 在此添加控件通知处理程序代码
    CString str;
    int idx = m_ListBox.GetCurSel();
    if(idx == LB_ERR) return;

    m_ListBox.GetText(idx, str);
    m_szCurSelected = str;
}

void OUChooseSkinDlg::OnLbnDblclkList1()
{
    // TODO: 在此添加控件通知处理程序代码
    CString str;
    int idx = m_ListBox.GetCurSel();
    if(idx == LB_ERR) return;

    m_ListBox.GetText(idx, str);
    m_szCurSelected = str;

    this->OnOK();
}

void OUChooseSkinDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    if(GetSelectedSkin() == "") this->OnCancel();
    else CDialog::OnOK();
}
