// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Menu.h"

Menu::Menu()
{
	m_iNextPos = 0;
	m_hMenu = CreateMenu();
};

Menu::~Menu()
{
	
};

void Menu::addCommand(UTF16 sText, int iCommand)
{
	MENUITEMINFO menuItemInfo;
	menuItemInfo.cbSize = sizeof(menuItemInfo);
	menuItemInfo.fType = MFT_STRING;
	menuItemInfo.dwTypeData = (LPWSTR) sText.ptr();
	menuItemInfo.cch = sText.length();
	menuItemInfo.fMask = MIIM_TYPE | MIIM_ID;
	menuItemInfo.wID = (UINT) iCommand;
	InsertMenuItem(m_hMenu, m_iNextPos++, TRUE, &menuItemInfo);
}

void Menu::addSeperator()
{
	MENUITEMINFO menuItemInfo;
	menuItemInfo.cbSize = sizeof(menuItemInfo);
	menuItemInfo.fType = MFT_SEPARATOR;
	menuItemInfo.fMask = MIIM_TYPE;
	InsertMenuItem(m_hMenu, m_iNextPos++, TRUE, &menuItemInfo);
}

void Menu::addSubmenu(UTF16 sText, HMenu hSubMenu)
{
	m_submenus.add(hSubMenu);

	MENUITEMINFO menuItemInfo;
	menuItemInfo.cbSize = sizeof(menuItemInfo);
	menuItemInfo.fType = MFT_STRING;
	menuItemInfo.dwTypeData = (LPWSTR) sText.ptr();
	menuItemInfo.cch = sText.length();
	menuItemInfo.hSubMenu = hSubMenu->m_hMenu;
	menuItemInfo.fMask = MIIM_TYPE | MIIM_SUBMENU;
	InsertMenuItem(m_hMenu, m_iNextPos++, TRUE, &menuItemInfo);
}

