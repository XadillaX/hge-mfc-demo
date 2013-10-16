#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
using namespace std;

enum OUASSETSTAB_LAYER
{
    OUAT_BOTTOM,
    OUAT_CARPET,
    OUAT_BUILDING,

    OUAT_MAX
};

struct OUASSETSITEM
{
    int                     id;
    string                  filename;
    CImage*                 img;
    CBitmap*                bitmap;
    int                     imgid;
};

#define OUASSETS_MAXID 1000

// OUAssetsTabDlg 对话框

class OUAssetsTabDlg : public CDialog
{
	DECLARE_DYNAMIC(OUAssetsTabDlg)

public:
	OUAssetsTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OUAssetsTabDlg();

// 对话框数据
	enum { IDD = IDD_ASSETSTABDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
    CListCtrl m_AssetsList;

    OUASSETSTAB_LAYER           m_emLayer;
    CString                     m_szPath;
    CString                     m_szFormat;
    vector<string>              m_arrExts;
    bool                        m_bInited;
    map<int, OUASSETSITEM>      m_Items;
    CImageList                  m_AssetsImages;
    static bool                 m_bFirstShow;

protected:
    CBitmap*                    ScaleBitmap(CImage* pImg, int maxwidth, int maxheight);

public:
    void InitInfo(OUASSETSTAB_LAYER emLayer, const char* szPath, const char* szFormat, vector<CString>& arrExts);
    void InsertToList(OUASSETSITEM& item);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
protected:
    CStatic m_AssetsCount;
public:
    afx_msg void OnDestroy();
    afx_msg void OnHdnItemchangedAssetslist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedAssetslist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
