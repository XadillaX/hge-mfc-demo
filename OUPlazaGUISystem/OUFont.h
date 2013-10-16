#ifndef __OUFONT_H__
#define __OUFONT_H__
#pragma once
#include <winbase.h>
#include <atlbase.h>
#include <map>
#include "Global.h"
#include "OUGUISingleton.h"
using namespace std;

#define C2W(buffer) (CA2WEX<1024>(buffer))
#define W2C(buffer) (CW2AEX<1024>(buffer))

class OUFontLoader;
class OUFont;

template class OUGUI_API OUGUISingleton<OUFontLoader>;
class OUGUI_API OUFontLoader : public OUGUISingleton<OUFontLoader>
{
public:
    OUFontLoader();

public:
    bool                        AddXmlLoader(string& szFilename);
    bool                        AddXmlLoaderByDoc(TiXmlDocument* pDoc);

    void                        Release();

    OUFont*                     GetFont(string& szFontname);

private:
    map<string, OUFont*>        m_Fonts;
};

class OUGUI_API OUFont
{
public:
	OUFont(const char* lpsFontName, int nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
	~OUFont(void);

public:
	// 渲染文本
	virtual void	Print( float x, float y, const char *format, ... );
	virtual void	Printf( float x, float y, const wchar_t *Str);
	virtual void	Render(float x, float y, const wchar_t* text );

	// 设置与获取颜色
	virtual void	SetColor( DWORD dwColor, int i = -1 );
	virtual DWORD	GetColor( int i = 0 );

	// 获取文本宽高
	virtual SIZE	GetTextSize( const wchar_t* text );

	// 根据坐标获取字符
	virtual wchar_t	GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y );

	// 设置字间距
	virtual void	SetKerningWidth( float kerning );
	virtual void	SetKerningHeight( float kerning );

	// 获取字间距
	virtual float	GetKerningWidth();
	virtual float	GetKerningHeight();

	// 字体大小
	virtual float	GetFontSize();
	
	// 获取字符宽度
	inline float	GetWidthFromCharacter( wchar_t c, bool original = false );

private:
	// 根据字符获取轮廓
	unsigned int	GetGlyphByCharacter( wchar_t c );
	inline void		CacheCharacter(unsigned int idx, wchar_t c);

	typedef struct tagEngineFontGlyph
	{
		HTEXTURE	t;
		float		w;
		float		h;
		float		x;
		float		y;
		float		c;
	}TENGINEFONTGLYPH;

	static const unsigned int font_count = 0xFFFF;// = sizeof(wchar_t);
	TENGINEFONTGLYPH	m_Glyphs[font_count];
	UINT				m_nAntialias;//反锯齿
	LONG				m_nAscent;//基线
	DWORD				m_dwFontColor;
	float				m_nFontSize;
	float				m_nKerningWidth;
	float				m_nKerningHeight;

	class HGE*			m_pHGE;
	class hgeSprite*	m_pSprite;

	// GDI设备
	HDC					m_hMemDC;
	HFONT				m_hFont;
};

#endif
