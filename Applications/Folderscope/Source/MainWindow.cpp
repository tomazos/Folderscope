// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "MainWindow.h"

#include "MainWindow.h"

#include "Runtime.h"
#include "TabView.h"
#include "LogView.h"
#include "WelcomeView.h"
#include "Menu.h"
#include "Controller.h"

#include "shlobj.h"

#include "TaskTracker.h"

#include "FolderscopeServerRPC.h"
#include "FolderImageView.h"
#include "SurfaceScanner.h"

#include "Application.h"

#include "PaintContext.h"

#define DeclCommand(sCommand,iCommand) static const int sCommand = iCommand;

DeclCommand(DCC_OPEN,1);
DeclCommand(DCC_CLOSE,2);
DeclCommand(DCC_REFRESH,3);
DeclCommand(DCC_IMPORT,4);
DeclCommand(DCC_QUIT,5);
DeclCommand(DCC_SORT_PATH,6);
DeclCommand(DCC_SORT_NAME,7);
DeclCommand(DCC_SORT_TYPE,8);
DeclCommand(DCC_SORT_SIZE,9);
DeclCommand(DCC_SORT_CONTENT,10);
DeclCommand(DCC_SORT_MODDATE,11);
DeclCommand(DCC_MARK_ALL,12);
DeclCommand(DCC_MARK_NONE,13);
DeclCommand(DCC_MARK_INVERT,14);
DeclCommand(DCC_MARK_DUPLICATES,15);
DeclCommand(DCC_MARK_MATCH_PATH,16);
DeclCommand(DCC_MARK_MATCH_CONTENT,17);
DeclCommand(DCC_DELETE,21);
DeclCommand(DCC_RENAME,22);
DeclCommand(DCC_COPY,23);
DeclCommand(DCC_EXTERNAL_COPY,24);
DeclCommand(DCC_EXTERNAL_MOVE,25);
DeclCommand(DCC_CHANGES_REMOVE,27);
DeclCommand(DCC_CHANGES_COMMIT,28);
DeclCommand(DCC_DEEP_SCAN,29);

MainWindow::MainWindow()
	: GUI(NULL, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
{
	m_hThis = this;
	setText(L"Folderscope 1.0");

	m_tabView = NewObject(MainTabView, this);
	m_hWelcomeView = m_tabView->getWelcomeView();

	Controller::get()->addListener(this);

	HMenu hMainMenu = NewObject(Menu);

	HMenu hFolderMenu = NewObject(Menu);
	hFolderMenu->addCommand(L"Open Folder...", DCC_OPEN);
	hFolderMenu->addCommand(L"Close Folder", DCC_CLOSE);
	hFolderMenu->addSeperator();
	hFolderMenu->addCommand(L"Deep Scan", DCC_DEEP_SCAN);
	hFolderMenu->addSeperator();
	hFolderMenu->addCommand(L"Refresh", DCC_REFRESH);
	hFolderMenu->addSeperator();
	hFolderMenu->addCommand(L"Import Files Into Folder...", DCC_IMPORT);
	hFolderMenu->addSeperator();
	hFolderMenu->addCommand(L"Quit", DCC_QUIT);
	hMainMenu->addSubmenu(L"Folder", hFolderMenu);

	HMenu hMarkMenu = NewObject(Menu);
	hMarkMenu->addCommand(L"Mark All Files", DCC_MARK_ALL);
	hMarkMenu->addCommand(L"Unmark All Files", DCC_MARK_NONE);
	hMarkMenu->addCommand(L"Invert Marks", DCC_MARK_INVERT);
	hMarkMenu->addSeperator();
	hMarkMenu->addCommand(L"Mark Duplicate Files", DCC_MARK_DUPLICATES);
	hMarkMenu->addSeperator();
	hMarkMenu->addCommand(L"Mark File Paths Matching...", DCC_MARK_MATCH_PATH);
	hMarkMenu->addCommand(L"Mark File Content Matching...", DCC_MARK_MATCH_CONTENT);
	hMainMenu->addSubmenu(L"Mark", hMarkMenu);

	HMenu hModifyMenu = NewObject(Menu);
	hModifyMenu->addCommand(L"Delete Marked Files", DCC_DELETE);
	hModifyMenu->addCommand(L"Rename/Move Marked Files...", DCC_RENAME);
	hModifyMenu->addCommand(L"Copy Marked Files...", DCC_COPY);
	hModifyMenu->addSeperator();
	hModifyMenu->addCommand(L"Copy Marked Files To External Folder...", DCC_EXTERNAL_COPY);
	hModifyMenu->addCommand(L"Move Marked Files To External Folder...", DCC_EXTERNAL_MOVE);
	hMainMenu->addSubmenu(L"Modify", hModifyMenu);

	HMenu hChangesMenu = NewObject(Menu);
	hChangesMenu->addCommand(L"Remove Selected Changes", DCC_CHANGES_REMOVE);
	hChangesMenu->addCommand(L"Commit Pending Changes", DCC_CHANGES_COMMIT);
	hMainMenu->addSubmenu(L"Changes", hChangesMenu);

	m_hMainMenu = hMainMenu;
	attachMenu(m_hMainMenu.ptr());

	show();
	update();
};

HWelcomeView MainWindow::getWelcomeView()
{
	return m_hWelcomeView;
}

MainWindow::~MainWindow()
{
	Controller::get()->removeListener(this);
};

Result MainWindow::msg_command_menu(int iCommand)
{
	switch (iCommand)
	{
	case DCC_OPEN: cmd_open(); return eOk;
	case DCC_CLOSE: cmd_close(); return eOk;
	case DCC_REFRESH: cmd_refresh(); return eOk;
	case DCC_IMPORT: cmd_import(); return eOk;
	case DCC_QUIT: cmd_quit(); return eOk;
    case DCC_MARK_ALL: cmd_mark_all(); return eOk;
    case DCC_MARK_NONE: cmd_mark_none(); return eOk;
    case DCC_MARK_INVERT: cmd_mark_invert(); return eOk;
    case DCC_MARK_DUPLICATES: cmd_mark_duplicates(); return eOk;
    case DCC_MARK_MATCH_PATH: cmd_mark_match_path(); return eOk;
    case DCC_MARK_MATCH_CONTENT: cmd_mark_match_content(); return eOk;
    case DCC_DELETE: cmd_delete(); return eOk;
    case DCC_RENAME: cmd_rename(); return eOk;
    case DCC_COPY: cmd_copy(); return eOk;
    case DCC_EXTERNAL_COPY: cmd_external_copy(); return eOk;
    case DCC_EXTERNAL_MOVE: cmd_external_move(); return eOk;
    case DCC_CHANGES_REMOVE: cmd_changes_remove(); return eOk;
    case DCC_CHANGES_COMMIT: cmd_changes_commit(); return eOk;
    case DCC_DEEP_SCAN: cmd_deep_scan(); return eOk;

	default:
		Log::errorf(L"Command %d not yet implemented", iCommand);
		return eFailed;
	}
}

Result MainWindow::msg_size(int iWidth, int iHeight)
{
	if (m_tabView.isSet())
		m_tabView->move(0, 0, iWidth, iHeight, true);

	return eOk;
};

Result MainWindow::msg_destroy()
{
	PostQuitMessage(0);
	return eOk;
};

Result MainWindow::msg_paint()
{
	PaintContext context(this);

	context.rect(Brush::eAppWorkspace, 0, 0, context.getWidth(), context.getHeight());
	return eOk;
}

void MainWindow::cmd_open()
{
	UTF16Buffer sbBrowsePath(MAX_PATH+2);

	BROWSEINFO binfo;
	Memory::zero(&binfo, sizeof(binfo));
	binfo.hwndOwner = m_hwnd;
	binfo.lpszTitle = L"Select folder to open...";
	binfo.ulFlags = BIF_USENEWUI;
	
	PIDLIST_ABSOLUTE pidl = SHBrowseForFolder(&binfo);

	if (pidl == NULL)
		return;

	if (!SHGetPathFromIDList(pidl, sbBrowsePath.ptr()))
		Log::errorf(L"!SHGetPathFromIDList(pidl, buf.ptr())");

	UTF16 sBrowsePath = sbBrowsePath;

	CoTaskMemFree(pidl);

	Error error;
	Path root(error, sBrowsePath);

	if (!error)
		Controller::get()->open(error, root);

	if (error)
		logerr("Could not open \"%s\" because \"%s\"", UTF16(error).ptr());
};

void MainWindow::cmd_quit()
{
	DestroyWindow(m_hwnd);
};

Result MainWindow::msg_user()
{
	TaskTracker::instance()->processTasks_MainThread();
	return eOk;
}

void MainWindow::tasksAvailable()
{
	PostMessage(m_hwnd, WM_USER, NULL, NULL);
}


void MainWindow::Controller_onAddImage(const HFolderImage& image)
{
	HFolderImageView view = NewObject(FolderImageView, m_tabView.ptr(), image);
	m_tabView->insertPane(m_tabView->getNumPanes() - 1, image->getShortRoot(), view);
}

void MainWindow::Controller_onDelImage(const HFolderImage& image)
{
	for (int i = 1; i < m_tabView->getNumPanes() - 1; i++)
	{
		HFolderImageView view = m_tabView->getPane(i).cast<FolderImageView>();
		if (view.isSet() && view->getImage() == image)
		{
			m_tabView->delIndex(i);
			return;
		}
	}
	Check();
}

void MainWindow::Controller_onFocusImage(const HFolderImage& image)
{
	for (int i = 1; i < m_tabView->getNumPanes() - 1; i++)
	{
		HFolderImageView view = m_tabView->getPane(i).cast<FolderImageView>();
		if (view.isSet() && view->getImage() == image)
		{
			m_tabView->setCurrentIndex(i);
			return;
		}
	}
}

void MainWindow::Controller_onBlurImage(const HFolderImage&)
{

}

void MainWindow::cmd_close()
{
	Controller::get()->close();
}

void MainWindow::cmd_deep_scan()
{
	Controller::get()->deep_scan();
}

void MainWindow::cmd_refresh()
{
	Controller::get()->refresh();
}

void MainWindow::cmd_import()
{
	Controller::get()->import();
}

void MainWindow::cmd_mark_all()
{
	Controller::get()->mark_all();
}

void MainWindow::cmd_mark_none()
{
	Controller::get()->mark_none();
}

void MainWindow::cmd_mark_invert()
{
	Controller::get()->mark_invert();
}

void MainWindow::cmd_mark_duplicates()
{
	Controller::get()->mark_duplicates();
}

void MainWindow::cmd_mark_match_path()
{
	Controller::get()->mark_match_path();
}

void MainWindow::cmd_mark_match_content()
{
	Controller::get()->mark_match_content();
}

void MainWindow::cmd_delete()
{
	Controller::get()->delete_();
}

void MainWindow::cmd_rename()
{
	Controller::get()->rename();
}

void MainWindow::cmd_copy()
{
	Controller::get()->copy();
}

void MainWindow::cmd_external_copy()
{
	Controller::get()->external_copy();
}

void MainWindow::cmd_external_move()
{
	Controller::get()->external_move();
}

void MainWindow::cmd_changes_remove()
{
	Controller::get()->changes_remove();
}

void MainWindow::cmd_changes_commit()
{
	Controller::get()->changes_commit();
}

