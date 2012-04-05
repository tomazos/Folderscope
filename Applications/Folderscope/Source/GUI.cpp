// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "GUI.h"

#include "Runtime.h"

#include "DrawingContext.h"

#include "commctrl.h"
#include "msgToStr.h"

HINSTANCE GUI::s_hinstance(NULL);

void GUI::register_()
{
	GlobalSync;

	static bool bRegistered(false);

	if (!bRegistered)
	{
		LoadLibrary(L"Riched20.dll");

		WNDCLASS wc;
		wc.style = 0; 
		wc.lpfnWndProc = (WNDPROC) DefWindowProc; 
		wc.cbClsExtra = 0; 
		wc.cbWndExtra = 0; 
		wc.hInstance = s_hinstance; 
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName =  NULL; 
		wc.lpszClassName = L"GUI";

		ATOM res = RegisterClass(&wc);

		Check(res != 0);

		bRegistered = true;
	}
}

GUI::GUI(GUI* pParent, DWORD dwStyle, ECursor eCursor, const UTF16& sClass)
	: m_hwnd(NULL)
	, m_wndproc(DefWindowProc)
	, m_eCursor(eCursor)
{
	register_();

	int iPos = 0;
	HMENU hMenu = 0;

	if (dwStyle & WS_OVERLAPPEDWINDOW)
		iPos = CW_USEDEFAULT;

	if (dwStyle & WS_CHILD)
	{
		static int childId(1);
		childId++;
		hMenu = (HMENU) childId;
	}
	
	m_hwnd = CreateWindowEx(0, sClass.ptr(), NULL, dwStyle, iPos, iPos, iPos, iPos,
		(pParent? pParent->m_hwnd : NULL), hMenu, s_hinstance, NULL);

	Check(m_hwnd != 0);

	SetWindowLong(m_hwnd, GWL_USERDATA, (LONG) this);
	m_wndproc = (WNDPROC) SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG) GUI::winproc);
}

void GUI::detach()
{
	if (m_hwnd != NULL)
	{
		SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG) m_wndproc);
		SetWindowLong(m_hwnd, GWL_USERDATA, (LONG) NULL);
		m_hwnd = NULL;
	}
}

int GUI::getHeight()
{
	RECT r;
	r.bottom = 0;
	BOOL iRes = GetClientRect(m_hwnd, &r);
	Check(iRes != 0);
	return r.bottom;
}

int GUI::getWidth()
{
	RECT r;
	r.right = 0;
	BOOL iRes = GetClientRect(m_hwnd, &r);
	Check(iRes != 0);
	return r.right;
}

void GUI::move(int iLeft, int iTop, int iWidth, int iHeight, bool bRepaint)
{
	if (0 == MoveWindow(m_hwnd, iLeft, iTop, iWidth, iHeight, (bRepaint ? TRUE : FALSE)))
	{
		Log::errorf(L"GUI(%p)::move(%d,%d,%d,%d,%s) failed",this,iLeft,iTop,iWidth,iHeight, (bRepaint? L"true" : L"false"));
	}
};

GUI* GUI::lookup(HWND hWnd)
{
	return (GUI*) GetWindowLong(hWnd, GWL_USERDATA);
};

void GUI::show()
{
	ShowWindow(m_hwnd, SW_SHOW);
}

void GUI::hide()
{
	ShowWindow(m_hwnd, SW_HIDE);
}

void GUI::update()
{
	UpdateWindow(m_hwnd);
}

void GUI::redraw()
{
	RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

void GUI::setFont(const Font& font)
{
	SendMessage(m_hwnd, WM_SETFONT, (WPARAM) font.get_HFONT(), MAKELPARAM(TRUE, 0));
}

Result GUI::processNotification_WM_COMMAND(UInt32 iNoteCode)
{
	switch (iNoteCode)
	{
	case BN_CLICKED: return note_clicked();
	case EN_SETFOCUS: return note_setfocus();
	case EN_KILLFOCUS: return note_killfocus();
	case EN_CHANGE: return note_change();
	default: return eDefault;
	}
}

Result GUI::processNotification_WM_NOTIFY(UInt32 iNoteCode)
{
	switch (iNoteCode)
	{
	case TCN_SELCHANGE: return note_selchange();
	default: return eDefault;
	}
}

void GUI::setText(const UTF16& sText)
{
	if (!SetWindowText(m_hwnd, sText.ptr()))
		logwarn("SetWindowText failed");
}

UTF16 GUI::getText()
{
	UTF16Buffer sbWindowText(GetWindowTextLength(m_hwnd) + 1);

	GetWindowText(m_hwnd, sbWindowText.ptr(), sbWindowText.flength());

	return sbWindowText;
};

void GUI::enable()
{
	EnableWindow(m_hwnd, true);
}

void GUI::disable()
{
	EnableWindow(m_hwnd, false);
}

int GUI::getTextWidth(const Font& font, const UTF16& s)
{
	DrawingContext context(this);
	return context.getTextWidth(font, s);
}

int GUI::getTextHeight(const Font& font, const UTF16& s)
{
	DrawingContext context(this);
	return context.getTextHeight(font, s);
}

void GUI::invalidate(int iLeft, int iTop, int iWidth, int iHeight)
{
	RECT r;
	r.top = iTop;
	r.bottom = iTop + iHeight;
	r.left = iLeft;
	r.right = iLeft + iWidth;
	InvalidateRect(m_hwnd, &r, false);
}

void GUI::invalidate()
{
	RECT r;
	GetClientRect(m_hwnd, &r);
	InvalidateRect(m_hwnd, &r,false);
}

void GUI::alert(const UTF16& sTitle, const UTF16& sMessage)
{
	MessageBox(NULL, sMessage.ptr(), sTitle.ptr(), MB_OK);
}

GUI::~GUI()
{
	if (m_hwnd != NULL)
	{
		detach();
		BOOL res = DestroyWindow(m_hwnd);
		if (res != 0)
		{
			UTF16 sError = WindowsError::getLastError();
			logerr("~GUI: %s", sError.ptr());
		}
	};
}

LRESULT CALLBACK GUI::winproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GUI* pThis = (GUI*) GetWindowLong(hwnd, GWL_USERDATA);

	if (pThis)
		return pThis->processMessage(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT GUI::processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Result eResult = eDefault;

	switch (uMsg) {
    case WM_SIZE:		eResult = msg_size(LOWORD(lParam), HIWORD(lParam)); break; 
    case WM_DESTROY:	eResult = msg_destroy(); break;
	case WM_NOTIFY:		eResult = msg_notify((NMHDR*) lParam); break;
	case WM_PAINT:		eResult = msg_paint(); break;
	case WM_COMMAND:	eResult = msg_command(HIWORD(wParam), LOWORD(wParam), (HWND) lParam); break;
	case WM_USER:		eResult = msg_user(); break;
	case WM_LBUTTONUP: eResult = msg_lbuttonup(SInt16(LOWORD(lParam)), SInt16(HIWORD(lParam))); break;
	case WM_LBUTTONDOWN: eResult = msg_lbuttondown(SInt16(LOWORD(lParam)), SInt16(HIWORD(lParam))); break;
	case WM_MOUSEMOVE: eResult = msg_mousemove(SInt16(LOWORD(lParam)), SInt16(HIWORD(lParam))); break;
	case WM_MOUSEHOVER: eResult = msg_mousehover(SInt16(LOWORD(lParam)), SInt16(HIWORD(lParam))); break;
	case WM_MOUSELEAVE: eResult = msg_mouseleave(); break;
	case WM_KEYDOWN: eResult = msg_keydown((UInt32)wParam, (UInt32)lParam); break;
	case WM_KEYUP: eResult = msg_keyup((UInt32)wParam, (UInt32)lParam); break;
	case WM_CHAR: eResult = msg_char((UInt32)wParam, (UInt32)lParam); break;
	case WM_PASTE: eResult = msg_paste(); break;
	case WM_VSCROLL: eResult = msg_vscroll(wParam); break;
	case WM_SETCURSOR: eResult = msg_setcursor(); break;
	default:			eResult = eDefault; break;
	}

	if (uMsg == WM_DESTROY)
	{
		LRESULT res = CallWindowProc(m_wndproc, m_hwnd, uMsg, wParam, lParam);
		detach();
		return res;
	}

	if (eResult == eDefault)
		return CallWindowProc(m_wndproc, m_hwnd, uMsg, wParam, lParam);

	if (eResult == eOk)
	{
		switch (uMsg)
		{
		case WM_SETCURSOR: return TRUE;
		default: return 0;
		}
	}

	Check();
	return -1;
};

Result GUI::msg_notify(NMHDR* p)
{
	if (p)
	{
		GUI* pWin = lookup(p->hwndFrom);

		if (pWin)
			return pWin->processNotification_WM_NOTIFY(p->code);
	}

	return eDefault;
};

Result GUI::msg_command(UInt16 iType, UInt16 iId, HWND hwnd)
{
	if (hwnd == NULL)
	{
		switch (iType)
		{
		case 0: return msg_command_menu(iId);
		case 1: return msg_command_accelerator(iId);
		default: return eFailed;
		}
	}

	GUI* pControl = lookup(hwnd);

	if (pControl)
	{
		return pControl->processNotification_WM_COMMAND(iType);
	}
	else
	{
		return eDefault;
	}
}

Result GUI::msg_vscroll(UInt32 wParam)
{
	int iMsg = LOWORD(wParam);
	switch (iMsg)
	{
	case SB_BOTTOM: return vscroll_bottom();
	case SB_ENDSCROLL: return vscroll_endscroll();
	case SB_LINEDOWN: return vscroll_linedown();
	case SB_LINEUP: return vscroll_lineup();
	case SB_PAGEDOWN: return vscroll_pagedown();
	case SB_PAGEUP: return vscroll_pageup();
	case SB_THUMBPOSITION: return vscroll_thumbposition(HIWORD(wParam));
	case SB_THUMBTRACK: return vscroll_thumbtrack(HIWORD(wParam));
	case SB_TOP: return vscroll_top();
	default: Check(); return eDefault;
	}
}

void GUI::attachMenu(Menu* menu)
{
	SetMenu(m_hwnd, menu->m_hMenu);
}

void GUI::setCursor(ECursor cursor)
{
	m_eCursor = cursor;
	if (m_eCursor != eDefaultCursor)
		setCursor();
}

void GUI::setCursor()
{
	LPCTSTR wincursor;
	switch (m_eCursor)
	{
	case eArrow: wincursor = IDC_ARROW; break;
	case eHand: wincursor = IDC_HAND; break;
	case eIBeam: wincursor = IDC_IBEAM; break;
	case eWait: wincursor = IDC_WAIT; break;
	case eDefaultCursor: default: Check(); return;
	}
	SetCursor(LoadCursor(NULL, wincursor));
}

Result GUI::msg_setcursor()
{
	if (m_eCursor == eDefaultCursor)
		return eDefault;

	setCursor();
	return eOk;
}
