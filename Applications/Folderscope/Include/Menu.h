// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class GUI;

DeclHandleable(Menu);
class Menu : public Handleable
{
public:
	Menu();
	~Menu();

	void addCommand(UTF16 sText, int iCommand = 0);
	void addSeperator();
	void addSubmenu(UTF16 sText, HMenu hSubMenu);

private:
	int m_iNextPos;
	HMENU m_hMenu;
	LinkedList<HMenu> m_submenus;

	friend class GUI;
};
