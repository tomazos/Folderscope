//#pragma once
//#include "global.h"
//
//#include "Basics.h"
//
//#define GUseFont(fontname) (FontProvider::get##fontname##Font())
//
//#define GDeclFont(fontname) public: static HFONT get##fontname##Font(); \
//	private: HFONT priv_get##fontname##Font(); HFONT m_##fontname##Font;
//
//class FontProvider
//{
//public:
//	GDeclFont(Big);
//	GDeclFont(Bold);
//	GDeclFont(Std);
//	GDeclFont(Fixed);
//	GDeclFont(Small);
//
//private:
//	FontProvider();
//
//	static FontProvider* instance();
//	static Lock s_lock;
//};
