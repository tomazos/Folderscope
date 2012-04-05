// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"

#include "MainTabView.h"
#include "Menu.h"
#include "WelcomeView.h"
#include "Controller.h"

#include "FolderscopeServerRPC.h"

#include "MainWindow.h"

DeclWeakHandleable(Application);
class Application : public WeakHandleable
{
public:
	static HLogListener s_hFileLog;

	Application();
	virtual ~Application();

	static HApplication instance() { return s_pInstance; }

	void browseFolder();

	int getNoOfRecentFolders() { return 0; }
	UTF16 getRecentFolder(int iIndex) { return L""; }

	bool isAutocheckEnabled();

	void checkVersion();
	FolderscopeServerRPC::HCheckVersionListener m_checkVersionListener;
	void checkVersionStart();
	void checkVersionSuccess(Version latest, UTF16 sUrl);
	void checkVersionFailed();

	void enableAutocheck();
	void disableAutocheck();

	Version getCurrentVersion();
	Version getLatestVersion();
	UTF16 getLatestURL();

	void upgrade();

	void subscribeNewsletter(UTF16 email);
	FolderscopeServerRPC::HSubscribeNewsletterListener m_subscribeNewsletterListener;
	void subscribeNewsletterStart();
	void subscribeNewsletterFailed();
	void subscribeNewsletterSuccess();

	UTF16 getEmailAddress();

	void preStartup();
	void postStartup();

	void tasksAvailable();

private:
	void loadCompiledIn();
	void loadSettings();
	void saveSettings();

	JSON m_jSettings;
	JSON m_jCompiledIn;
	Version m_latestVersion;
	UTF16 m_sLatestVersionURL;

	static HApplication s_pInstance;

	static void registerWindow();

	HMainWindow m_hMainWindow;
	HWelcomeView m_hWelcomeView;

	Path m_settingsFile;
	Path m_logFile;
};

