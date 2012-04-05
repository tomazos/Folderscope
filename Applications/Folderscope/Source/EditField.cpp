// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "EditField.h"

#include "CommCtrl.h"
#include "Richedit.h"

#include "FontProvider.h"

#include "Windowsx.h"

EditField::EditField(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL, eDefaultCursor, L"Edit")
{
	setFont(Font::eStandard);
}

void EditField::setCueBanner(const UTF16& sBanner)
{
	Edit_SetCueBannerText(m_hwnd, sBanner.ptr());
}

EditField::~EditField()
{

}

Result EditField::note_setfocus()
{
	onFocus();
	return eOk;
}

Result EditField::note_killfocus()
{
	onBlur();
	return eOk;
}

Result EditField::note_change()
{
	onChange();
	return eOk;
}

void EditField::getSel(int& iStartSel, int& iEndSel)
{
	DWORD iSel = Edit_GetSel(m_hwnd);
	iEndSel = HIWORD(iSel);
	iStartSel = LOWORD(iSel);
}

void EditField::setSel(int iStartSel, int iEndSel)
{
	Edit_SetSel(m_hwnd, UInt16(iStartSel), UInt16(iEndSel));
}
