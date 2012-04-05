// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"

#include "MainTabView.h"
#include "Menu.h"
#include "WelcomeView.h"
#include "Controller.h"

DeclWeakHandleable(MainWindow);
class MainWindow : public GUI, public Controller::Listener
{
public:
	MainWindow();
	virtual ~MainWindow();

	virtual Result msg_size(int iWidth, int iHeight);
	virtual Result msg_destroy();
	virtual Result msg_command_menu(int iCommand);
	virtual Result msg_user();
	virtual Result msg_paint();

	void cmd_open();
	void cmd_close();
	void cmd_deep_scan();
	void cmd_refresh();
	void cmd_import();
	void cmd_quit();
    void cmd_mark_all();
    void cmd_mark_none();
    void cmd_mark_invert();
    void cmd_mark_duplicates();
    void cmd_mark_match_path();
    void cmd_mark_match_content();
    void cmd_delete();
    void cmd_rename();
    void cmd_copy();
    void cmd_external_copy();
    void cmd_external_move();
    void cmd_changes_remove();
    void cmd_changes_commit();

	void tasksAvailable();

	virtual void Controller_onAddImage(const HFolderImage&);
	virtual void Controller_onDelImage(const HFolderImage&);
	virtual void Controller_onFocusImage(const HFolderImage&);
	virtual void Controller_onBlurImage(const HFolderImage&);

	HWelcomeView getWelcomeView();

private:
	HMainWindow m_hThis;

	HMainTabView m_tabView;
	HMenu m_hMainMenu;
	HWelcomeView m_hWelcomeView;
};

