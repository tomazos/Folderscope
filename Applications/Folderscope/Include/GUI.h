// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Result.h"
#include "Menu.h"

#include "Font.h"

class Context;
class PaintContext;
class DrawingContext;

DeclHandleable(GUI);
class GUI : public WeakHandleable
{
public:
	enum ECursor
	{
		eArrow,
		eHand,
		eIBeam,
		eWait,
		eDefaultCursor
	};

	GUI(GUI* pParent, DWORD dwStyle, ECursor eCursor = eDefaultCursor, const UTF16& sClass = L"GUI");
	virtual ~GUI();

	virtual void move(int iLeft, int iTop, int iWidth, int iHeight, bool bRepaint);

	virtual void show();
	virtual void hide();
	virtual void enable();
	virtual void disable();
	virtual void update();
	virtual void redraw();

	virtual UTF16 getText();
	virtual void setText(const UTF16& sText);
	virtual void setFont(const Font&);
	virtual void setCursor(ECursor);

	virtual Result processNotification_WM_COMMAND(UInt32 iNoteCode);
	virtual Result processNotification_WM_NOTIFY(UInt32 iNoteCode);

	virtual Result note_clicked() { return eDefault; }
	virtual Result note_selchange() { return eDefault; }
	virtual Result note_setfocus() { return eDefault; }
	virtual Result note_killfocus() { return eDefault; }
	virtual Result note_change() { return eDefault; }

	int getHeight();
	int getWidth();

	int getTextWidth(const Font&, const UTF16& s);
	int getTextHeight(const Font&, const UTF16& s);

	void invalidate(int iLeft, int iTop, int iWidth, int iHeight);
	void invalidate();

	static void alert(const UTF16& sTitle, const UTF16& sMessage);

	static LRESULT CALLBACK winproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual Result msg_size(int iWidth, int iHeight) { return eDefault; };
	virtual Result msg_destroy() { return eDefault; };
	virtual Result msg_notify(NMHDR* p);
	virtual Result msg_paint() { return eDefault; }
	virtual Result msg_command(UInt16 iType, UInt16 iId, HWND hwnd);
	virtual Result msg_command_menu(int iCommand) { return eDefault; }
	virtual Result msg_command_accelerator(int iCommand) { return eDefault; }
	virtual Result msg_user() { return eDefault; }
	virtual Result msg_lbuttonup(int iX, int iY) { return eDefault; }
	virtual Result msg_lbuttondown(int iX, int iY) { return eDefault; }
	virtual Result msg_mousemove(int iX, int iY) { return eDefault; }
	virtual Result msg_mousehover(int iX, int iY) { return eDefault; }
	virtual Result msg_mouseleave() { return eDefault; }
	virtual Result msg_paste() { return eDefault; }
	virtual Result msg_vscroll(UInt32);
	virtual Result msg_char(UInt32, UInt32) { return eDefault; }
	virtual Result msg_keydown(UInt32 iCharCode, UInt32 iExtraInfo) { return eDefault; }
	virtual Result msg_keyup(UInt32 iCharCode, UInt32 iExtraInfo) { return eDefault; }
	virtual Result msg_setcursor();
	virtual Result vscroll_bottom() { return eDefault; }
	virtual Result vscroll_endscroll() { return eDefault; }
	virtual Result vscroll_linedown() { return eDefault; }
	virtual Result vscroll_lineup() { return eDefault; }
	virtual Result vscroll_pagedown() { return eDefault; }
	virtual Result vscroll_pageup() { return eDefault; }
	virtual Result vscroll_thumbposition(UInt16 iPos) { return eDefault; }
	virtual Result vscroll_thumbtrack(UInt16 iPos) { return eDefault; }
	virtual Result vscroll_top() { return eDefault; }

protected:
	void attachMenu(Menu* menu);

	static GUI* lookup(HWND hWnd);

	ECursor m_eCursor;
	HWND m_hwnd;
	WNDPROC m_wndproc;
	static HINSTANCE s_hinstance;

private:

	void detach();

	virtual void setCursor();

	static void register_();

	friend class Context;
	friend class PaintContext;
	friend class DrawingContext;

	friend int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int);
};
