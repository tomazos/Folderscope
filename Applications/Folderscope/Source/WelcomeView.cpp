// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "WelcomeView.h"

#include "Runtime.h"

#include "FontProvider.h"

#include "DrawingContext.h"
#include "PaintContext.h"
#include "MemoryContext.h"
#include "Application.h"
#include "Controller.h"

#include "shellapi.h"

#include "OffscreenContext.h"

class UpgradeButton : public Button
{
public:
	UpgradeButton(WelcomeView* pWelcome)
		: Button(pWelcome)
		, m_pWelcome(pWelcome)
	{}

	virtual void onClick()
	{
		if (getText() == L"Check")
		{
			disable();
			setText(L"Upgrade");
			m_pWelcome->checkVersion();
		}
		else
			m_pWelcome->upgrade();
	}
private:
	WelcomeView* m_pWelcome;
};

class SubscribeButton : public Button
{
public:
	SubscribeButton(WelcomeView* pWelcome)
		: Button(pWelcome)
		, m_pWelcome(pWelcome)
	{}

	virtual void onClick()
	{
		m_pWelcome->subscribe();
	}

private:
	WelcomeView* m_pWelcome;
};

DeclHandleable(BrowseButton);
class BrowseButton : public Button
{
public:
	BrowseButton(WelcomeView* pWelcome)
		: Button(pWelcome)
		, m_pWelcome(pWelcome)
	{}

	enum EState
	{
		eBrowse,
		eOpen
	};

	EState m_state;

	void setBrowse()
	{
		enable();
		m_state = eBrowse;
		setText(L"Browse");
	}

	void setOpen()
	{
		enable();
		m_state = eOpen;
		setText(L"Open");
	}

	void setCreate()
	{
		enable();
		m_state = eOpen;
		setText(L"Create");
	}

	void setInvalid()
	{
		disable();
		setText(L"Invalid");
	}

	virtual void onClick()
	{
		switch (m_state)
		{
		case eBrowse: m_pWelcome->browseFolder(); break;
		case eOpen: m_pWelcome->openFolder(); break;
		}
	}

private:
	WelcomeView* m_pWelcome;
};

class AutocheckCheckbox : public CheckBox
{
public:
	AutocheckCheckbox(WelcomeView* pWelcome)
		: CheckBox(pWelcome, pWelcome->isAutocheckEnabled())
		, m_pWelcome(pWelcome)
	{}

	virtual void onCheck()
	{
		m_pWelcome->m_upgradeButton->setText(L"Upgrade");
		m_pWelcome->m_upgradeButton->disable();
		m_pWelcome->checkVersion();
		m_pWelcome->enableAutocheck();
	}

	virtual void onUncheck()
	{
		m_pWelcome->disableAutocheck();
	}

private:
	WelcomeView* m_pWelcome;
};


class UrlLink : public Link
{
public:
	UrlLink(WelcomeView* pWelcome, UTF16 sString, UTF16 sUrl)
		: Link(pWelcome, sString)
		, m_pWelcome(pWelcome)
		, m_sUrl(sUrl)
	{}

	virtual void onClick()
	{
		Error error;
		OperatingSystem::open(error, m_sUrl);
		if (error)
			logerr("Unable to open \"%s\" because \"%s\"", m_sUrl.ptr(), UTF16(error).ptr());
	}

private:
	WelcomeView* m_pWelcome;
	UTF16 m_sUrl;
};

class EmailEditField : public EditField
{
public:
	EmailEditField(WelcomeView* pWelcome)
		: EditField(pWelcome)
		, m_pWelcome(pWelcome)
	{}

private:
	bool m_bBlockOnChange;

	virtual void onChange()
	{
		UTF16 sText = getText();
		if (sText.contains(L"@"))
		{
			HButton subscribeButton = m_pWelcome->m_subscribeButton;
			if (subscribeButton.isSet())
			{
				m_pWelcome->m_subscribeButton->enable();
				m_pWelcome->m_subscribeButton->setText(L"Subscribe");
			}
		}
		else
		{
			HButton subscribeButton = m_pWelcome->m_subscribeButton;
			if (subscribeButton.isSet())
			{
				m_pWelcome->m_subscribeButton->disable();
			}
		}
	}

	WelcomeView* m_pWelcome;
};

class FolderEditField : public EditField
{
public:
	FolderEditField(WelcomeView* pWelcome)
		: EditField(pWelcome)
		, m_pWelcome(pWelcome)
		, m_bBlockOnChange(false)
	{}

	void onUpArrow()
	{
		m_pWelcome->onUpArrow();
	}

	void onDownArrow()
	{
		m_pWelcome->onDownArrow();
	}

	void onEnterKey()
	{
		m_pWelcome->onEnterKey();
	}

	virtual Result msg_keydown(UInt32 iCharCode, UInt32 iExtraInfo)
	{
		switch (iCharCode)
		{
		case VK_UP: onUpArrow(); return eOk;
		case VK_DOWN: onDownArrow(); return eOk;
		default: return eDefault;
		}
	}

	virtual Result msg_keyup(UInt32 iCharCode, UInt32 iExtraInfo)
	{
		return eDefault;
	}

	virtual Result msg_char(UInt32 iCharCode, UInt32 iExtraInfo)
	{
		switch (iCharCode)
		{
		case L'\r' : onEnterKey(); return eOk;
		default: return eDefault;
		}
	}

	void pathSelected(const UTF16& sSelectedPath)
	{
		m_bBlockOnChange = true;

		UTF16 sText = getText();
		int iStartSel, iEndSel;
		getSel(iStartSel, iEndSel);
		setText(sSelectedPath);
		setSel(iStartSel, sSelectedPath.length());

		m_bBlockOnChange = false;

		invalidate();
	}

private:
	bool m_bBlockOnChange;

	virtual void onChange()
	{
		UTF16 sText = getText();

		if (!m_bBlockOnChange)
		{
			if (m_pWelcome->m_folderAssist.isSet())
			{
				int iStartSel, iEndSel;
				getSel(iStartSel, iEndSel);
				m_pWelcome->m_folderAssist->setPrefix(sText.prefix(iStartSel));
			}
		}

		if (sText.empty())
		{
			HBrowseButton browseButton = (BrowseButton*) m_pWelcome->m_browseButton.ptr();
			if (browseButton.isSet())
			{
				browseButton->setBrowse();
			}
		}
		else
		{
			Error error;

			Path pt(error, sText);

			if (error)
			{
				HBrowseButton browseButton = (BrowseButton*) m_pWelcome->m_browseButton.ptr();
				if (browseButton.isSet())
				{
					browseButton->setInvalid();
				}
				return;
			}

			bool bExists = pt.exists(error);

			if (error)
			{
				HBrowseButton browseButton = (BrowseButton*) m_pWelcome->m_browseButton.ptr();
				if (browseButton.isSet())
				{
					browseButton->setInvalid();
				}
				return;
			}

			if (bExists)
			{
				HBrowseButton browseButton = (BrowseButton*) m_pWelcome->m_browseButton.ptr();
				if (browseButton.isSet())
				{
					browseButton->setOpen();
				}
			}
			else
			{
				HBrowseButton browseButton = (BrowseButton*) m_pWelcome->m_browseButton.ptr();
				if (browseButton.isSet())
				{
					browseButton->setCreate();
				}
			}
		}
	}

	WelcomeView* m_pWelcome;
};

WelcomeView::WelcomeView(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_CLIPCHILDREN)
	, m_bChecking(false)
	, m_sCopyrightInfo(L"(c) 2009 Folderscope.com, Tomazos-Jager Software Labs, Andrew Tomazos")
{
	setup();

	m_upgradeButton = NewObject(UpgradeButton, this);
	if (Application::instance()->isAutocheckEnabled())
	{
		m_upgradeButton->setText(L"Upgrade");
		m_upgradeButton->disable();
	}
	else
	{
		m_upgradeButton->setText(L"Check");
		m_upgradeButton->enable();
	}

	m_subscribeButton = NewObject(SubscribeButton, this);

	m_emailField = NewObject(EmailEditField, this);
	m_emailField->setCueBanner(L"Type Email Address");
	UTF16 sEmailAddress = Application::instance()->getEmailAddress();
	m_emailField->setText(sEmailAddress);
	m_subscribeButton->disable();
	if (sEmailAddress.empty())
		m_subscribeButton->setText(L"Subscribe");
	else
		m_subscribeButton->setText(L"Subscribed");	

	m_autocheckCheckbox = NewObject(AutocheckCheckbox, this);
	
	m_websiteLink = NewObject(UrlLink, this, L"www.folderscope.com", L"http://www.folderscope.com");
	m_supportLink = NewObject(UrlLink, this, L"support@folderscope.com", L"mailto:support@folderscope.com");
	m_registerLink = NewObject(UrlLink, this, L"register this copy $19", L"http://register.folderscope.com");

	BrowseButton* browseButton = NewObject(BrowseButton, this);
	browseButton->setBrowse();
	m_browseButton = browseButton;

	m_folderField = NewObject(FolderEditField, this);
	m_folderField->setCueBanner(L"Enter folder to open here, or click Browse");

	m_folderAssist = NewObject(FolderAssist, this, m_iUnit);
}

void WelcomeView::setup()
{
	DrawingContext context(this);

	m_iSideTextWidth = getTextWidth(Font::eBold, L"Folderscope Newsletter");
	m_iUnit = getTextHeight(Font::eBold, L"Folderscope Newsletter");

	m_iHalfUnit = m_iUnit / 2 + 3;
	m_iUnit = 2 * m_iHalfUnit;

	m_iLineSpace = m_iUnit + m_iHalfUnit;
	m_iLeftSideBox = m_iUnit;
	m_iLeftSideText = m_iLeftSideBox + m_iUnit;
	m_iRightSideText = m_iLeftSideText + m_iSideTextWidth;
	m_iRightSideBox = m_iRightSideText + m_iUnit;
	m_iLeftMainText = m_iRightSideBox + m_iUnit;
	m_iSideBoxWidth = m_iRightSideBox - m_iLeftSideBox;

	m_iSideBox1Top = m_iUnit;
	m_iSideText1Top = m_iSideBox1Top + m_iUnit;
	m_iSideText1Bottom = m_iSideText1Top + m_iLineSpace* 3 + m_iUnit;
	m_iSideBox1Bottom = m_iSideText1Bottom + m_iUnit;

	m_iSideBox2Top = m_iSideBox1Bottom + m_iUnit;
	m_iSideText2Top = m_iSideBox2Top + m_iUnit;
	m_iSideText2Bottom = m_iSideText2Top + m_iLineSpace* 2 + m_iUnit;
	m_iSideBox2Bottom = m_iSideText2Bottom + m_iUnit;

	m_iSideBox3Top = m_iSideBox2Bottom + m_iUnit;
	m_iSideText3Top = m_iSideBox3Top + m_iUnit;
	m_iSideText3Bottom = m_iSideText3Top + m_iLineSpace* 2 + m_iUnit;
	m_iSideBox3Bottom = m_iSideText3Bottom + m_iUnit;

	m_iSideBox1Height = m_iSideBox1Bottom - m_iSideBox1Top;
	m_iSideText1Height = m_iSideText1Bottom - m_iSideText1Top;
	m_iSideBox2Height = m_iSideBox2Bottom - m_iSideBox2Top;
	m_iSideText2Height = m_iSideText2Bottom - m_iSideText2Top;
	m_iSideBox3Height = m_iSideBox3Bottom - m_iSideBox3Top;
	m_iSideText3Height = m_iSideText3Bottom - m_iSideText3Top;

	m_iTitleTop = m_iUnit;
	m_iTitleBottom = m_iTitleTop + 	getTextHeight(Font::eBig, L"Folderscope 1.0");
	m_iFolderFieldTop = m_iTitleBottom + m_iUnit;
	m_iFolderFieldBottom = m_iFolderFieldTop + m_iUnit;

	m_iFolderAssistLeft = m_iLeftMainText;
	m_iFolderAssistTop = m_iFolderFieldBottom + m_iHalfUnit;

	m_iCopyrightInfoWidth = getTextWidth(Font::eStandard, m_sCopyrightInfo);

};

Result WelcomeView::msg_size(int iWidth, int iHeight)
{
	m_iBrowseButtonRight = iWidth - m_iUnit;
	m_iBrowseButtonLeft = m_iBrowseButtonRight - getTextWidth(Font::eStandard, L"Browse") - 2 * m_iUnit;
	m_iFolderFieldRight = m_iBrowseButtonLeft - m_iUnit;

	m_iFolderAssistWidth = m_iBrowseButtonRight - m_iFolderAssistLeft;
	m_iFolderAssistHeight = iHeight - 2 * m_iUnit - m_iFolderAssistTop;

	m_autocheckCheckbox->move(m_iLeftSideText, m_iSideText1Top + 3 * m_iLineSpace, m_iUnit, m_iUnit, true);

	m_upgradeButton->move(m_iLeftSideText + m_iSideTextWidth/2,
		m_iSideText1Top + 3 * m_iLineSpace - 2,
		m_iRightSideText - (m_iLeftSideText + m_iSideTextWidth/2), m_iUnit + 4, true);

	m_emailField->move(m_iLeftSideText, m_iSideText2Top + m_iLineSpace,
		m_iSideTextWidth,
		m_iUnit, true);

	m_subscribeButton->move(m_iLeftSideText + m_iSideTextWidth/2,
		m_iSideText2Top + 2 * m_iLineSpace - 2,
		m_iRightSideText - (m_iLeftSideText + m_iSideTextWidth/2), m_iUnit + 4, true);

	m_registerLink->move(m_iLeftSideText, m_iSideText3Top + 2 * m_iLineSpace, m_iSideTextWidth, m_iUnit, true);
	m_websiteLink-> move(m_iLeftSideText, m_iSideText3Top + 0 * m_iLineSpace, m_iSideTextWidth, m_iUnit, true);
	m_supportLink-> move(m_iLeftSideText, m_iSideText3Top + 1 * m_iLineSpace, m_iSideTextWidth, m_iUnit, true);

	m_folderField->move(m_iLeftMainText, m_iFolderFieldTop, m_iFolderFieldRight - m_iLeftMainText, m_iUnit, true);
	m_browseButton->move(m_iBrowseButtonLeft, m_iFolderFieldTop, m_iBrowseButtonRight - m_iBrowseButtonLeft, m_iUnit, true);

	m_folderAssist->move(m_iFolderAssistLeft, m_iFolderAssistTop, m_iFolderAssistWidth, m_iFolderAssistHeight, true);
	return eOk;
};


Result WelcomeView::msg_paint()
{
	PaintContext pcontext(this);
	OffscreenContext context(&pcontext);

	context.setFont(Font::eStandard);

	context.rect(Brush::eWindow, 0, 0, context.getWidth(), context.getHeight());

	#define BoxHeightLines(iLines) (m_iUnit + (m_iUnit + m_iHalfUnit) * (iLines) + m_iHalfUnit)

	context.rect(Brush::eActiveBorder, m_iLeftSideBox, m_iUnit, m_iSideBoxWidth, BoxHeightLines(4));

	context.setFont(Font::eBig);
	context.text(L"Folderscope 1.0", m_iLeftMainText, m_iUnit);

	context.setFont(Font::eBold);
	context.text(L"Version Info", m_iLeftSideText, m_iUnit * 2);

	UTF16 sCurrentVersion = Application::instance()->getCurrentVersion();
	
	Version latestVersion = Application::instance()->getLatestVersion();

	UTF16 sLatestVersion;

	if (m_bChecking)
	{
		sLatestVersion = L"checking...";
	}
	else
	{
		if (latestVersion.isValid())
			sLatestVersion = UTF16(latestVersion);
		else
			sLatestVersion = L"unknown";
	}

	context.setFont(Font::eStandard);
	context.text(L"This:", m_iLeftSideText, m_iUnit*2 + 1 * m_iLineSpace);

	int iCurrentVersionWidth, iCurrentVersionHeight;
	context.calcTextSize(sCurrentVersion, iCurrentVersionWidth, iCurrentVersionHeight);
	context.text(sCurrentVersion, m_iRightSideText - iCurrentVersionWidth, m_iUnit*2 + 1 * m_iLineSpace);

	context.text(L"Latest:", m_iLeftSideText, m_iUnit*2 + 2 * m_iLineSpace);
	
	int iLatestVersionWidth, iLatestVersionHeight;
	context.calcTextSize(sLatestVersion, iLatestVersionWidth, iLatestVersionHeight);
	context.text(sLatestVersion, m_iRightSideText-iLatestVersionWidth, m_iUnit*2 + 2 * m_iLineSpace);
	
	context.text(L"auto", m_iLeftSideText + m_iUnit, m_iUnit*2 + 3 * m_iLineSpace);

	context.rect(Brush::eActiveBorder, m_iLeftSideBox, m_iSideBox2Top, m_iSideBoxWidth, m_iSideBox2Height);

	context.setFont(Font::eBold);
	context.text(L"Folderscope Newsletter", m_iLeftSideText, 2* m_iUnit + BoxHeightLines(4) + m_iUnit);

	context.setFont(Font::eStandard);
	context.text(L"Email:", m_iLeftSideText, 2* m_iUnit + BoxHeightLines(4) + m_iUnit+ 1 * m_iLineSpace);

	context.rect(Brush::eActiveBorder, m_iLeftSideBox, m_iSideBox3Top, m_iSideBoxWidth, m_iSideBox3Height);

	context.text(m_sCopyrightInfo, (context.getWidth() - m_iCopyrightInfoWidth)/2, context.getHeight() - 3 * m_iHalfUnit);

	context.blit();
	return eOk;
}

bool WelcomeView::isAutocheckEnabled()
{
	return Application::instance()->isAutocheckEnabled();
}

void WelcomeView::checkVersion()
{
	Application::instance()->checkVersion();
}

void WelcomeView::enableAutocheck()
{
	Application::instance()->enableAutocheck();
}

void WelcomeView::disableAutocheck()
{
	Application::instance()->disableAutocheck();
}

void WelcomeView::checkVersionSuccess(Version latest, UTF16 sUrl)
{
	m_bChecking = false;
	invalidate(m_iLeftSideBox, m_iUnit, m_iSideBoxWidth, BoxHeightLines(4));
	if (latest > Application::instance()->getCurrentVersion())
		m_upgradeButton->enable();
}

void WelcomeView::checkVersionStart()
{
	m_bChecking = true;
}

void WelcomeView::checkVersionFailed()
{
	m_bChecking = false;
}

void WelcomeView::upgrade()
{
	Application::instance()->upgrade();
}

void WelcomeView::subscribe()
{
	UTF16 email = m_emailField->getText();

	Application::instance()->subscribeNewsletter(email);
}

void WelcomeView::subscribeNewsletterStart()
{
	m_subscribeButton->disable();	
	m_subscribeButton->setText(L"working");
	loginfo("Sending newsletter subscription request...");
}

void WelcomeView::subscribeNewsletterFailed()
{
	m_subscribeButton->enable();	
	m_subscribeButton->setText(L"Retry");
	logwarn("Newsletter subscription failed, most likely due to a network error.");
	alert(L"Newsletter Subscription Failed", L"Failed to subscribe to newsletter, most\r\nlikely a network error.  Check activity log.");
}

void WelcomeView::subscribeNewsletterSuccess()
{
	m_subscribeButton->disable();
	m_subscribeButton->setText(L"subscribed");
	loginfo("Newsletter Subscription Success");
	alert(L"Newsletter Subscribe Success", L"You are now subscribed to\r\nthe Folderscope Newsletter");
}

void WelcomeView::browseFolder()
{
	Application::instance()->browseFolder();
}

void WelcomeView::openFolder()
{
	Error error;
	Path root(error, m_folderField->getText());

	if (!error)
		Controller::get()->open(error, root);

	if (error)
		logerr("Unable to open \"%s\" because \"%s\"", m_folderField->getText().ptr(), UTF16(error).ptr());
}

void
WelcomeView::onUpArrow()
{
	if (m_folderAssist.isSet())
		m_folderAssist->onUpArrow();
}

void
WelcomeView::onDownArrow()
{
	if (m_folderAssist.isSet())
		m_folderAssist->onDownArrow();
}

void
WelcomeView::onEnterKey()
{
	if (m_folderAssist.isSet())
		m_folderAssist->onEnterKey();
}

void WelcomeView::pathSelected(const UTF16& sSelectedPath)
{
	m_folderField->pathSelected(sSelectedPath);
}

void WelcomeView::promoteCurrentSelection()
{
	int iStartSel, iEndSel;

	m_folderField->getSel(iStartSel, iEndSel);
	iStartSel = iEndSel;
	m_folderField->setSel(iStartSel, iEndSel);
}

void WelcomeView::clickBrowseButton()
{
	m_browseButton->click();
}
