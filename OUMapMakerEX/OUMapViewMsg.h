#ifndef __OUMAPVIEWMSG_H__
#define __OUMAPVIEWMSG_H__

enum OUMAPVIEWMSG
{
    OM_SIZE,                ///< �ı䴰���С
    OM_NEWMAP,              ///< �½���ͼ
    OM_SELECTTILE,          ///< �޸ĵ�ǰͼ�����
    OM_CHANGEEDITMODE,      ///< �޸ı༭ģʽ
    OM_SCROLLMOVEAREA,      ///< �������ƶ���ͼ
    OM_SETINNERFONT,        ///< �����ڲ���������

    OM_MAX
};

/** �ı䴰���С���� */
struct OM_SIZE_PARAM
{
    int                     m_nScreenWidth;
    int                     m_nScreenHeight;
};

/** �½���ͼ���� */
struct OM_NEWMAP_PARAM
{
    char                    m_szName[32];
    int                     m_nWidth;
    int                     m_nHeight;

    int                     m_nTileWidth;
    int                     m_nTileHeight;
};

/** ѡ��ͼ����� */
struct OM_SELECTTILE_PARAM
{
    int                     m_emResType;
    int                     m_nID;
};

/** �޸ı༭ģʽ���� */
struct OM_CHANGEEDITMODE_PARAM
{
    int                     m_emEditMode;
};

/** �����ƶ���ͼ */
struct OM_SCROLLMOVEAREA_PARAM
{
    int                     m_nToX;
    int                     m_nToY;
};

/** �ڲ��������� */
struct OM_SETINNERFONT_PARAM
{
    char                    m_szFont[64];

    int                     m_nSize;
    bool                    m_bBold;
    bool                    m_bItalic;
    bool                    m_bAntialias;
};

#endif
