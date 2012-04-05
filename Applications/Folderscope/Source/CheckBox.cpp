// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "CheckBox.h"

CheckBox::CheckBox(GUI* pParent, bool bInitiallyChecked)
	: GUI(pParent, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, eDefaultCursor, L"BUTTON")
{
	SendMessage(m_hwnd, BM_SETCHECK, (bInitiallyChecked ? BST_CHECKED : BST_UNCHECKED), 0);
}

CheckBox::~CheckBox()
{

}

bool CheckBox::isChecked()
{
	return SendMessage(m_hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void CheckBox::check()
{
	SendMessage(m_hwnd, BM_SETCHECK, BST_CHECKED, 0);
	onCheck();
}

void CheckBox::uncheck()
{
	SendMessage(m_hwnd, BM_SETCHECK, BST_UNCHECKED, 0);
	onUncheck();
}

Result CheckBox::note_clicked()
{
	if (isChecked())
		uncheck();
	else
		check();

	return eOk;
}
