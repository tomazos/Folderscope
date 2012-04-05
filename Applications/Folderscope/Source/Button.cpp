// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Button.h"

#include "FontProvider.h"

Button::Button(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, eDefaultCursor, L"BUTTON")
{
	setFont(Font::eStandard);
}

Button::~Button()
{

}

void Button::click()
{
	SendMessage(m_hwnd, BM_CLICK, 0, 0);
}

Result Button::note_clicked()
{
	onClick();
	return eOk;
}
