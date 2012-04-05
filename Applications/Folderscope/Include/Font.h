// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "DrawingObject.h"

class Font
{
public:
	enum EFontName
	{
		eStandard,
		eBold,
		eSmall,
		eBig,
		eFixed
	};
	Font(EFontName);

private:
	void init();
	void checkInit();

	HFONT m_font;
	HFONT get_HFONT() const;
	friend class Context;
	friend class GUI;
};
