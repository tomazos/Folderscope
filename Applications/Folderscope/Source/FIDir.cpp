// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FIDir.h"

#include "Runtime.h"

#include "FIFile.h"

const Chr16* g_sUTF16_dir(L"folder");

FIDir::FIDir(FIDir* pParent, HComponents components)
	: FIItem(components, g_sUTF16_dir, 0, TimePoint::foreverFuture(), TimePoint::foreverPast())
	, m_pParent(pParent)
	, m_iNumFiles(0)
	, m_iNumMarkedFiles(0)
{
	if (pParent != NULL)
		pParent->addChild(this);
}

static void componentCheck(FIItem::HComponents parent, FIItem::HComponents child)
{
	bool bOk = (parent->size() == child->size() - 1);
	for (int i = 0; i < parent->size(); i++)
	{
		if (compareUTF16CaseInsensetive((*parent)[i], (*child)[i]) != 0)
			bOk = false;
	}
	if (!bOk)
	{
		logwarn("Found invalid path match...");
		for (int i = 0; i < parent->size(); i++)
		{
			loginfo("parent[%d] = %s", i, (*parent)[i].ptr());
		}
		for (int i = 0; i < child->size(); i++)
		{
			loginfo("child[%d] = %s", i, (*child)[i].ptr());
		}
	}
}

void FIDir::addChild(FIItem* fi)
{
	componentCheck(getComponents(), fi->getComponents());

	m_pChildren += fi;

	addDescendant(fi);
}

void FIDir::addDescendant(FIItem* fi)
{
	if (dynamic_cast<FIFile*>(fi))
		m_iNumFiles++;

	m_iFileSize += fi->m_iFileSize;

	if (fi->m_created < m_created)
		m_created = fi->m_created;

	if (fi->m_modified > m_modified)
		m_modified = fi->m_modified;

	if (m_pParent != NULL)
		m_pParent->addDescendant(fi);
}

void FIDir::detachFromParent()
{
	if (m_pParent)
	{
		m_pParent->removeChild(this);
	}
}

void FIDir::removeChild(FIItem* fi)
{
	m_pChildren -= fi;
}

void FIDir::markDescendant()
{
	m_iNumMarkedFiles++;

	if (m_pParent != NULL)
		m_pParent->markDescendant();
}

void FIDir::unmarkDescendant()
{
	m_iNumMarkedFiles--;

	if (m_pParent != NULL)
		m_pParent->unmarkDescendant();
}

void FIDir::mark()
{
	TreeSet<FIItem*>::Iterator it(m_pChildren);
	FIItem* pItem;
	while (it(pItem))
		pItem->mark();
}

void FIDir::toggleMark()
{
	if (m_iNumMarkedFiles < m_iNumFiles)
		mark();
	else
		unmark();
}

void FIDir::unmark()
{
	TreeSet<FIItem*>::Iterator it(m_pChildren);
	FIItem* pItem;
	while (it(pItem))
		pItem->unmark();
}

Real64 FIDir::getMarkedState()
{
	if (m_iNumMarkedFiles == m_iNumFiles)
		return 1;
	else if (m_iNumMarkedFiles == 0)
		return 0;
	else
		return (Real64(m_iNumMarkedFiles) / Real64(m_iNumFiles));
}

UInt32 FIDir::getLink()
{
	return 0xFFFFFFFFU;
}

UTF16 FIDir::getLinkShortString()
{
	return "n/a";
}

UTF16 FIDir::getLinkLongString()
{
	return "n/a";
}

UInt32 FIDir::getContent()
{
	return 0xFFFFFFFFU;
}

UTF16 FIDir::getContentShortString()
{
	return "n/a";
}

UTF16 FIDir::getContentLongString()
{
	return "n/a";
}
