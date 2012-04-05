// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Font.h"

HFONT g_FontStandard;
HFONT g_FontBold;
HFONT g_FontSmall;
HFONT g_FontBig;
HFONT g_FontFixed;

void Font::init()
{
	HFONT defaultGUIFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

	g_FontStandard = defaultGUIFont;

	LOGFONT logFont;
	GetObject(defaultGUIFont, sizeof(logFont), &logFont);
	logFont.lfWeight = 700;
	logFont.lfWidth = 0;
	g_FontBold = CreateFontIndirect(&logFont);

	GetObject(defaultGUIFont, sizeof(logFont), &logFont);
	logFont.lfHeight = logFont.lfHeight * 2;
	logFont.lfWidth = 0;
	g_FontBig = CreateFontIndirect(&logFont);

	GetObject(defaultGUIFont, sizeof(logFont), &logFont);
	logFont.lfHeight = (logFont.lfHeight * 3) / 4;
	logFont.lfWidth = 0;
	g_FontSmall = CreateFontIndirect(&logFont);

	g_FontFixed = CreateFont(16,0,0,0,0,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, L"Courier New");
};

void Font::checkInit()
{
	GlobalSync;

	static bool bInitialized(false);
	if (!bInitialized)
	{
		bInitialized = true;

		init();
	}
}

Font::Font(EFontName eFontName)
{
	checkInit();
	switch (eFontName)
	{
	case eStandard: m_font = g_FontStandard; return;
	case eBold: m_font = g_FontBold; return;
	case eSmall: m_font = g_FontSmall; return;
	case eBig: m_font = g_FontBig; return;
	case eFixed: m_font = g_FontFixed; return;
	default: Check();
	}
};

HFONT Font::get_HFONT() const
{
	return m_font;
}