//#include "FontProvider.h"
//
//Lock FontProvider::s_lock;
//
//#define GDefnFont(fontname) \
//	HFONT FontProvider::get##fontname##Font() \
//    { Synchronizer sync(s_lock); return FontProvider::instance()->priv_get##fontname##Font(); } \
//	HFONT FontProvider::priv_get##fontname##Font() \
//	{ return m_##fontname##Font; }
//
//#define GHookFont(fontname) m_##fontname##Font
//
//GDefnFont(Big);
//GDefnFont(Bold);
//GDefnFont(Std);
//GDefnFont(Fixed);
//GDefnFont(Small);
//
//FontProvider*
//FontProvider::instance()
//{
//	static FontProvider* p (NULL);
//
//	if (!p)
//		p = new FontProvider();
//
//	return p;
//}
//
//FontProvider::FontProvider()
//{
//	GHookFont(Std) = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
//
//	LOGFONT logFont;
//	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(logFont), &logFont);
//	logFont.lfWeight = 700;
//	logFont.lfWidth = 0;
//	GHookFont(Bold) = (HFONT) CreateFontIndirect(&logFont);
//	
//	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(logFont), &logFont);
//	logFont.lfHeight = logFont.lfHeight * 2;
//	logFont.lfWidth = 0;
//	GHookFont(Big) = (HFONT) CreateFontIndirect(&logFont);
//
//	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(logFont), &logFont);
//	logFont.lfHeight = (logFont.lfHeight * 3) / 4;
//	logFont.lfWidth = 0;
//	GHookFont(Small) = (HFONT) CreateFontIndirect(&logFont);
//
//	GHookFont(Fixed) = CreateFont(16,0,0,0,0,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
//		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
//		FIXED_PITCH | FF_MODERN, L"Courier New");
//}
