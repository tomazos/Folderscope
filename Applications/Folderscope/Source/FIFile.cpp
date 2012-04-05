// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FIFile.h"

#include "FIDir.h"

#include "FIIndex.h"

FIFile::FIFile(FIDir* pParent, HComponents components, UTF16 sExt, UInt64 iFileSize, TimePoint created, TimePoint modified)
	: FIItem(components, sExt, iFileSize, created, modified)
	, m_pParent(pParent)
	, m_bMarked(false)
	, m_pIndex(NULL)
{
	pParent->addChild(this);
}

void FIFile::mark()
{
	if (!m_bMarked)
	{
		m_bMarked = true;
		m_pParent->markDescendant();
	}
}

void FIFile::toggleMark()
{
	if (m_bMarked)
		unmark();
	else
		mark();
}

void FIFile::unmark()
{
	if (m_bMarked)
	{
		m_bMarked = false;
		m_pParent->unmarkDescendant();
	}
}

Real64 FIFile::getMarkedState()
{
	if (m_bMarked)
		return 1;
	else
		return 0;
}

UInt32 FIFile::getLink()
{
	if (m_pIndex)
		return m_pIndex->getLink();
	else
		return 0xFFFFFFFEU;
}

UTF16 FIFile::getLinkShortString()
{
	if (m_pIndex)
		return m_pIndex->getLinkShortString();
	else
		return L"unscanned";
}

UTF16 FIFile::getLinkLongString()
{
	if (m_pIndex)
		return m_pIndex->getLinkLongString();
	else
		return "Unknown.  Need a Deep Scan.";
}

UInt32 FIFile::getContent()
{
	if (m_pIndex)
		return m_pIndex->getContent();
	else
		return 0xFFFFFFFEU;
}

UTF16 FIFile::getContentShortString()
{
	if (m_pIndex)
		return m_pIndex->getContentShortString();
	else
		return L"unscanned";
}

UTF16 FIFile::getContentLongString()
{
	if (m_pIndex)
		return m_pIndex->getContentLongString();
	else
		return "Unknown.  Need a Deep Scan.";
}