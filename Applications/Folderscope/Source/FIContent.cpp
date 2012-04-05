// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FIContent.h"

UInt32 FIContent::getContent()
{
	return m_iContentGroup;
}

UTF16 FIContent::getContentShortString()
{
	if (m_iContentGroup == 0xFFFFFFFDU)
		return L"unique";
	else
		return UTF16::format(L"#%u", m_iContentGroup);
}

UTF16 FIContent::getContentLongString()
{
	if (m_iContentGroup == 0xFFFFFFFDU)
		return UTF16(L"Unique Content: SHA2-512 = ") + UTF16(HexString::binToHex(m_sha));
	else
		return UTF16::format(L"Content Group #%u: SHA2-512 = ", m_iContentGroup) + UTF16(HexString::binToHex(m_sha));
}

void FIContent::setContentGroup(UInt32& iNextContentGroup)
{
	if (m_pFIIndexes.size() > 1)
	{
		m_iContentGroup = iNextContentGroup;
		iNextContentGroup++;
	}
	else
	{
		m_iContentGroup = 0xFFFFFFFDU;
	}
}
