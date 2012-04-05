// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FIIndex.h"

#include "FIContent.h"

FIIndex::FIIndex()
	: m_pContent(NULL)
	, m_iLinkGroup(0)
{}

void FIIndex::setFileIndexInfo(const FileIndexInfo& info)
{
	m_info = info;
}

void FIIndex::addFIFile(FIFile* pFile)
{
	m_pFIFiles.push(pFile);
}

void FIIndex::setFIContent(FIContent* pContent)
{
	m_pContent = pContent;
}

UInt32 FIIndex::getLink()
{
	return m_iLinkGroup;
}

UTF16 FIIndex::getLinkShortString()
{
	if (m_iLinkGroup == 0xFFFFFFFDU)
		return L"unique";
	else
		return UTF16::format(L"#%u", m_iLinkGroup);
}

UTF16 FIIndex::getLinkLongString()
{
	if (m_iLinkGroup == 0xFFFFFFFDU)
		return UTF16(L"Unique Link: ") + m_info.getString();
	else
		return UTF16::format(L"Link Group #%u: ", m_iLinkGroup) + m_info.getString();
}

UInt32 FIIndex::getContent()
{
	if (m_pContent)
		return m_pContent->getContent();
	else
		return 0xFFFFFFFEU;
}

UTF16 FIIndex::getContentShortString()
{
	if (m_pContent)
		return m_pContent->getContentShortString();
	else
		return L"unscanned";
}

UTF16 FIIndex::getContentLongString()
{
	if (m_pContent)
		return m_pContent->getContentLongString();
	else
		return L"unscanned";
}

void FIIndex::setLinkGroup(UInt32& iNextLinkGroup)
{
	if (m_pFIFiles.size() > 1)
	{
		m_iLinkGroup = iNextLinkGroup;
		iNextLinkGroup++;
	}
	else
	{
		m_iLinkGroup = 0xFFFFFFFDU;
	}
}
