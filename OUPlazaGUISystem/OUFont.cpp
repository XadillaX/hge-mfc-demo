#include "StdAfx.h"
#include "OUFont.h"
#include <atlbase.h>
#include <stdio.h>
#include "../OUPlazaRender/OUStringFunc.h"
#include "../OUPlazaRender/OUResourceManager.h"

OUGUISINGLETON_IMPL(OUFontLoader);

OUFontLoader::OUFontLoader()
{
    m_Fonts["_default"] = new OUFont("宋体", 12, 0, 0, 0);
}

void OUFontLoader::Release()
{
    for(map<string, OUFont*>::iterator it = m_Fonts.begin(); it != m_Fonts.end(); it++)
    {
        delete it->second;
    }

    m_Fonts.clear();
}

bool OUFontLoader::AddXmlLoader(string& szFilename)
{
    OUPACKFILEDATA pData = OUResourcePool::instance().GetResource(szFilename);
    if(0 == pData.m_dwFileSize)
    {
        return false;
    }

    TiXmlDocument* pDoc = new TiXmlDocument();

    pDoc->Parse((const char*)pData.m_pBuf, 0, TiXmlEncoding::TIXML_ENCODING_LEGACY);
    bool res = AddXmlLoaderByDoc(pDoc);
    delete pDoc;
    delete [](pData.m_pBuf);

    return res;
}

bool OUFontLoader::AddXmlLoaderByDoc(TiXmlDocument *pDoc)
{
    TiXmlElement* pRoot = pDoc->RootElement();
    TiXmlElement* pFontCfg = pRoot->FirstChildElement();
    
    /** 把所有字体都拿过来 */
    OUStringFunc szName;
    while(pFontCfg)
    {
        szName.SetString(string(pFontCfg->Value()));
        szName.ToUppercase();
        if(szName.GetString() == "FONT")
        {
            string name = "_default";
            string font = "宋体";
            int size = 12;
            bool bold = false;
            bool italic = false;
            bool antialias = true;

            /** 读取字体 */
            if(NULL != pFontCfg->Attribute("font"))
            {
                font = pFontCfg->Attribute("font");
            }

            /** 字体名 */
            if(NULL != pFontCfg->Attribute("name"))
            {
                name = pFontCfg->Attribute("name");
            }

            /** 字体大小 */
            if(TIXML_SUCCESS != pFontCfg->QueryIntAttribute("size", &size))
            {
                size = 12;
            }

            /** 是否粗体 */
            if(TIXML_SUCCESS != pFontCfg->QueryBoolAttribute("bold", &bold))
            {
                bold = false;
            }

            /** 是否斜体 */
            if(TIXML_SUCCESS != pFontCfg->QueryBoolAttribute("italic", &italic))
            {
                italic = false;
            }

            /** 是否反锯齿 */
            if(TIXML_SUCCESS != pFontCfg->QueryBoolAttribute("antialias", &antialias))
            {
                antialias = false;
            }

            if(m_Fonts.find(name) != m_Fonts.end())
            {
                delete m_Fonts[name];
                m_Fonts[name] = NULL;
            }

            m_Fonts[name] = new OUFont(font.c_str(), size, bold, italic, antialias);
        }
        
        pFontCfg = pFontCfg->NextSiblingElement();
    }

    return true;
}

OUFont* OUFontLoader::GetFont(string& szFontname)
{
    if(m_Fonts.find(szFontname) == m_Fonts.end()) return m_Fonts["_default"];

    return m_Fonts[szFontname];
}

///////////////////////////////////////////

__inline float _floor(float f)
{
	static int _n;
	_asm fld f
	_asm fistp _n
	return (float)_n;
}

// 65级灰度表
const unsigned char g_byAlphaLevel[65] = 
{
	  0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
	 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100,
	104,108,112,116,120,124,128,132,136,140,144,148,152,
	156,160,164,168,172,176,180,184,188,192,196,200,204,
	208,212,216,220,224,228,232,236,240,244,248,252,255
};

OUFont::OUFont(const char* lpsFontName, int nFaceSize, BOOL bBold, BOOL bItalic, BOOL bAntialias)
{
	m_pHGE = hgeCreate(HGE_VERSION);

	// 创建GDI相关设备
	HDC hDC = GetDC(m_pHGE->System_GetState(HGE_HWND));
	m_hMemDC = CreateCompatibleDC(hDC);
	if (NULL == m_hMemDC) return;
	ReleaseDC(m_pHGE->System_GetState(HGE_HWND),hDC);

	::SetMapMode(m_hMemDC, MM_TEXT);
	::SetTextColor(m_hMemDC,RGB(255,255,255));
	::SetBkColor(m_hMemDC,RGB(0,0,0));

	m_hFont = CreateFontA(
		-nFaceSize,
		0,
		0,
		0,
		(bBold) ? FW_BOLD : FW_NORMAL,
		bItalic,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		lpsFontName);
	if (NULL == (m_hFont)) return;
	SelectObject(m_hMemDC, m_hFont);

	memset(m_Glyphs,0,sizeof(TENGINEFONTGLYPH)*font_count);

	m_nAntialias	= bAntialias ? GGO_GRAY8_BITMAP : GGO_BITMAP;

	TEXTMETRIC tm;
	::GetTextMetrics(m_hMemDC,&tm);
	m_nAscent		= tm.tmAscent;

	m_nFontSize		= static_cast<float>(nFaceSize);
	m_nKerningWidth	= 0;
	m_nKerningHeight= 0;

	m_pSprite = new hgeSprite(0, 0, 0, 0, 0);
	m_pSprite->SetColor(ARGB(255, 255, 255, 255));
}

OUFont::~OUFont(void)
{
	for (int nIdx = 0; nIdx < font_count; ++nIdx)
	{ if (m_Glyphs[nIdx].t) m_pHGE->Texture_Free(m_Glyphs[nIdx].t); }

	if ((m_hFont)) DeleteObject(m_hFont);
	if ((m_hMemDC)) DeleteDC(m_hMemDC);

	if(m_pSprite) delete m_pSprite;
	if(m_pHGE) m_pHGE->Release();
}

// 渲染文本
void OUFont::Print( float x, float y, const char *format, ... )
{
	char sBuffer[10240] = {0};
	char *lpsArg=(char*)&format+sizeof(format);
	vsprintf_s(sBuffer, format, lpsArg);

	//Render(x,y,CA2W(sBuffer));
	Render(x,y,CA2WEX<1024>(sBuffer)); 
}

// 渲染文本
void OUFont::Printf( float x, float y, const wchar_t *Str)
{
	Render(x,y,Str); 
}

void OUFont::Render(float x, float y, const wchar_t* text )
{
	float offsetX = x;
	float offsetY = y;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
		}
		else
		{
			unsigned int idx = GetGlyphByCharacter(*text);
			if (idx > 0)
			{
				m_pSprite->SetTexture(m_Glyphs[idx].t);
				m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
				m_pSprite->Render(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y);
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
			else
			{
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
		}

		++text;
	}
}

// 设置与获取颜色
void OUFont::SetColor( DWORD dwColor, int i )
{
	m_pSprite->SetColor(dwColor,i);
}

DWORD OUFont::GetColor(int i)
{
	return m_pSprite->GetColor(i);
}

// 获取文本宽高
SIZE OUFont::GetTextSize( const wchar_t* text )
{
	SIZE dim = {0, static_cast<LONG>(m_nFontSize)};
	float nRowWidth = 0;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			dim.cy += static_cast<LONG>(m_nFontSize + m_nKerningHeight);
			if (dim.cx < static_cast<LONG>(nRowWidth))
				dim.cx = static_cast<LONG>(nRowWidth);
			nRowWidth = 0;
		}
		else
			nRowWidth += (GetWidthFromCharacter(*text) + m_nKerningWidth);
		++text;
	}

	if (dim.cx < static_cast<LONG>(nRowWidth))
		dim.cx = static_cast<LONG>(nRowWidth);

	return dim;
}

// 根据坐标获取字符
wchar_t OUFont::GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y )
{
	float x = 0;
	float y = 0;

	while (*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			x = 0;
			y += (m_nFontSize+m_nKerningHeight);
			text++;

			if (!(*text))
				break;
		}

		float w = GetWidthFromCharacter(*text);
		if (pixel_x > x && pixel_x <= x + w &&
			pixel_y > y && pixel_y <= y + m_nFontSize)
			return *text;

		x += (w+m_nKerningWidth);

		text++;
	}

	return L'\0';
}

// 设置字间距
void OUFont::SetKerningWidth( float kerning )
{
	m_nKerningWidth = kerning;
}
void OUFont::SetKerningHeight( float kerning )
{
	m_nKerningHeight = kerning;
}

// 获取字间距
float OUFont::GetKerningWidth()
{
	return m_nKerningWidth;
}
float OUFont::GetKerningHeight()
{
	return m_nKerningHeight;
}	

// 字体大小
float OUFont::GetFontSize()
{
	return m_nFontSize;
}

// 根据字符获取轮廓
unsigned int OUFont::GetGlyphByCharacter( wchar_t c )
{
	unsigned int idx = (unsigned int)c;
	if (NULL == (m_Glyphs[idx].t)) CacheCharacter(idx,c);
	return idx;
}
inline float OUFont::GetWidthFromCharacter( wchar_t c, bool original )
{
	unsigned int idx = GetGlyphByCharacter(c);
	if (original && idx > 0 && idx < font_count) return m_Glyphs[idx].c;
	return	(idx >= 0x2000) ? m_nFontSize : _floor(m_nFontSize / 2);
}
inline void OUFont::CacheCharacter(unsigned int idx, wchar_t c)
{
	if (idx < font_count && NULL == m_Glyphs[idx].t)
	{
		UINT nChar = (UINT)c;

		MAT2 mat2 = {{0,1},{0,0},{0,0},{0,1}};
		GLYPHMETRICS gm;
		DWORD nLen = ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,0,NULL,&mat2);

		HTEXTURE hTex = m_pHGE->Texture_Create(gm.gmBlackBoxX,gm.gmBlackBoxY);
		if (NULL == hTex) return;

		if((signed)nLen > 0)
		{
			LPBYTE lpBuf = new BYTE[nLen];

			if (nLen == ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,nLen,lpBuf,&mat2))
			{
				BYTE*	lpSrc = lpBuf;
				DWORD*	lpDst = m_pHGE->Texture_Lock(hTex,FALSE);

				if (GGO_BITMAP == m_nAntialias)
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 32 + (gm.gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							for(UINT k = 0; k < 8; ++k)   
							{
								UINT i = 8 * x + k;
								if (i >= gm.gmBlackBoxX)
								{
									x+=7;
									break;
								}
								lpDst[i] = ((lpSrc[x] >> (7 - k)) & 1) ? 0xFFFFFFFF : 0x0;
							}
						}

						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
					}
				}
				else
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 4 + (gm.gmBlackBoxX % 4 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							lpDst[x] = ARGB(g_byAlphaLevel[lpSrc[x]],0xFF,0xFF,0xFF);
						}

						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
					}
				}

				m_pHGE->Texture_Unlock(hTex);
			}

			delete []lpBuf;
		}
		else
		{
			// 非正常显示字符
		}

		m_Glyphs[idx].t = hTex;
		m_Glyphs[idx].w = static_cast<float>(gm.gmBlackBoxX);
		m_Glyphs[idx].h = static_cast<float>(gm.gmBlackBoxY);
		m_Glyphs[idx].x = static_cast<float>(-gm.gmptGlyphOrigin.x);
		m_Glyphs[idx].y = static_cast<float>(-m_nAscent + gm.gmptGlyphOrigin.y);
		m_Glyphs[idx].c = static_cast<float>(gm.gmCellIncX);
	}
}
