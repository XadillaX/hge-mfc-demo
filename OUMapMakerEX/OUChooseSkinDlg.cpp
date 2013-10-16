// OUChooseSkinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OUMapMakerEX.h"
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
    DDX_Control(pDX, IDC_SKINLIST, m_ListBox);
}


BEGIN_MESSAGE_MAP(OUChooseSkinDlg, CDialog)
    ON_LBN_DBLCLK(IDC_SKINLIST, &OUChooseSkinDlg::OnLbnDblclkSkinlist)
    ON_BN_CLICKED(IDC_CLEAN, &OUChooseSkinDlg::OnBnClickedClean)
END_MESSAGE_MAP()


// OUChooseSkinDlg 消息处理程序

BOOL OUChooseSkinDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
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

void OUChooseSkinDlg::OnLbnDblclkSkinlist()
{
    // TODO: 在此添加控件通知处理程序代码
    CString str;
    int idx = m_ListBox.GetCurSel();
    if(idx == LB_ERR) return;

    m_ListBox.GetText(idx, str);
    m_szCurSelected = str;

    OnOK();
}

void OUChooseSkinDlg::OnBnClickedClean()
{
    // TODO: 在此添加控件通知处理程序代码
    m_szCurSelected = "";
    OnOK();
}

string OUChooseSkinDlg::GetSelectedSkin()
{
    return m_szCurSelected;
}
