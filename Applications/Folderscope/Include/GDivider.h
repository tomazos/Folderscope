#pragma once
#include "global.h"

#include "GUI.h"

#include "Runtime.h"

class GDivider : public GUI
{
public:
	GDivider(GUI* pParent);

	virtual ~GDivider() {}

	virtual GResult msg_size(int iParentWidth, int iParentHeight);

	typedef void (*ChildPosCalculator) (
		int iParentWidth, int iParentHeight,
		int& iLeft, int& iTop, int& iWidth, int& iHeight);

	void addChild(HGUI hChild, ChildPosCalculator pos);

private:
	static void registerWindow();
	class ChildInfo
	{
	public:
		HGUI m_win;
		ChildPosCalculator m_pos;
	};

	LinkedList<ChildInfo> m_children;
};
