// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"

#include "Button.h"
#include "CheckBox.h"
#include "EditField.h"
#include "Link.h"
#include "FolderAssist.h"

DeclWeakHandleable(FolderEditField);

DeclWeakHandleable(WelcomeView);
class WelcomeView : public GUI
{
public:
	WelcomeView(GUI* pParent);

	virtual Result msg_size(int iWidth, int iHeight);
	virtual Result msg_paint();
	
	void upgrade();
	void subscribe();

	bool isAutocheckEnabled();

	void checkVersion();
	void enableAutocheck();
	void disableAutocheck();

	void checkVersionSuccess(Version latest, UTF16 sUrl);
	void checkVersionStart();
	void checkVersionFailed();
	bool m_bChecking;

	void subscribeNewsletterStart();
	void subscribeNewsletterFailed();
	void subscribeNewsletterSuccess();

	void browseFolder();
	void openFolder();

	void setup();

	void onUpArrow();
	void onDownArrow();
	void onEnterKey();

	void pathSelected(const UTF16& sSelectedPath);
	void promoteCurrentSelection();
	void clickBrowseButton();

	HButton m_upgradeButton, m_subscribeButton;
	HCheckBox m_autocheckCheckbox;
	HEditField m_emailField;
	HFolderEditField m_folderField;
	HButton m_browseButton;
	HLink m_registerLink, m_websiteLink, m_supportLink;
	HLink m_openFolderLink;
	HFolderAssist m_folderAssist;

	int m_iUnit;
	int m_iHalfUnit;
	int m_iLineSpace;
	int m_iLeftSideBox;
	int m_iLeftSideText;
	int m_iRightSideText;
	int m_iRightSideBox;
	int m_iLeftMainText;
	int m_iSideBoxWidth;
	int m_iSideTextWidth;

	int m_iSideBox1Top;
	int m_iSideText1Top;
	int m_iSideText1Bottom;
	int m_iSideBox1Bottom;

	int m_iSideBox2Top;
	int m_iSideText2Top;
	int m_iSideText2Bottom;
	int m_iSideBox2Bottom;

	int m_iSideBox3Top;
	int m_iSideText3Top;
	int m_iSideText3Bottom;
	int m_iSideBox3Bottom;

	int m_iSideBox1Height;
	int m_iSideText1Height;

	int m_iSideBox2Height;
	int m_iSideText2Height;

	int m_iSideBox3Height;
	int m_iSideText3Height;

	int m_iFolderFieldRight;
	int m_iBrowseButtonLeft;
	int m_iBrowseButtonRight;

	int m_iTitleTop;
	int m_iTitleBottom;
	int m_iFolderFieldTop;
	int m_iFolderFieldBottom;
	
	int m_iFolderAssistLeft;
	int m_iFolderAssistTop;
	int m_iFolderAssistWidth;
	int m_iFolderAssistHeight;

	UTF16 m_sCopyrightInfo;
	Size m_iCopyrightInfoWidth;

	void registerWindow();
};
