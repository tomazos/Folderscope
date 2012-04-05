// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Context.h"

#include "GUI.h"

class PaintContext: public Context
{
public:
	PaintContext(GUI* pWindow);
	~PaintContext();

private:
	HWND m_hwnd;
	PAINTSTRUCT m_ps;
};