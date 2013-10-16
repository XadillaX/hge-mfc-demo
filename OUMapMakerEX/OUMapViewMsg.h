#ifndef __OUMAPVIEWMSG_H__
#define __OUMAPVIEWMSG_H__

enum OUMAPVIEWMSG
{
    OM_SIZE,                ///< 改变窗体大小
    OM_NEWMAP,              ///< 新建地图
    OM_SELECTTILE,          ///< 修改当前图块参数
    OM_CHANGEEDITMODE,      ///< 修改编辑模式
    OM_SCROLLMOVEAREA,      ///< 滚动条移动地图
    OM_SETINNERFONT,        ///< 设置内部字体名称

    OM_MAX
};

/** 改变窗体大小参数 */
struct OM_SIZE_PARAM
{
    int                     m_nScreenWidth;
    int                     m_nScreenHeight;
};

/** 新建地图参数 */
struct OM_NEWMAP_PARAM
{
    char                    m_szName[32];
    int                     m_nWidth;
    int                     m_nHeight;

    int                     m_nTileWidth;
    int                     m_nTileHeight;
};

/** 选择图块参数 */
struct OM_SELECTTILE_PARAM
{
    int                     m_emResType;
    int                     m_nID;
};

/** 修改编辑模式参数 */
struct OM_CHANGEEDITMODE_PARAM
{
    int                     m_emEditMode;
};

/** 滚动移动地图 */
struct OM_SCROLLMOVEAREA_PARAM
{
    int                     m_nToX;
    int                     m_nToY;
};

/** 内部字体名称 */
struct OM_SETINNERFONT_PARAM
{
    char                    m_szFont[64];

    int                     m_nSize;
    bool                    m_bBold;
    bool                    m_bItalic;
    bool                    m_bAntialias;
};

#endif
