// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FIItem.h"

#include "Stream/FileSize.h"

#include "shellapi.h"

static UTF16 fileExtensionToFileTypeName(const UTF16& sExt)
{
	static TreeMap<UTF16, UTF16> s_typenames;

	if (s_typenames.has(sExt))
		return s_typenames.get(sExt);
	else
	{
		UTF16 sTypename;

		SHFILEINFO sfi;
		Memory::zero(&sfi, sizeof(sfi));
		DWORD_PTR dwp = SHGetFileInfo((UTF16(L".") + sExt).ptr(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
			SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
		if (dwp == 0)
		{
			Check();
			sTypename = sExt + L" File";
		}
		else
		{
			sTypename = UTF16(sfi.szTypeName);
		}
		s_typenames.add(sExt, sTypename);
		return sTypename;
	}
}

FIItem::FIItem(HComponents components, UTF16 sExt, UInt64 iFileSize, TimePoint created, TimePoint modified)
	: m_components(components)
	, m_sExt(sExt)
	, m_iFileSize(iFileSize)
	, m_created(created)
	, m_modified(modified)
{
	if (sExt.length() < 5)
		m_sExtShort = sExt;
	else
	{
		UTF16Buffer sExtShort(1);
		sExtShort << sExt.prefix(3);
		sExtShort << L"..";
		m_sExtShort = sExtShort;
	}

	if (m_sExtLong == L"folder")
	{
		m_sExt = L"folder";
		m_sExtShort = L"folder";
		m_sExtLong = L"Directory";
	}
	else
		m_sExtLong = fileExtensionToFileTypeName(sExt);
}

Path FIItem::getAbsolutePath(Error& error, const UTF16& sRoot)
{
	if (m_components->size() <= 0)
		return Path(error, sRoot);

	UTF16Buffer sPath(1);
	sPath << sRoot;
	sPath << L'\\';

	for (int i = 0; i < m_components->size() - 1; i++)
	{
		sPath << (*m_components)[i] << L'\\';
	}
	sPath << (*m_components)[m_components->size() - 1];
	return Path(error, sPath);
}

UTF16 FIItem::getPath()
{
	if (m_components->size() <= 0)
		return UTF16(L"\\");

	UTF16Buffer sPath(1);
	for (int i = 0; i < m_components->size() - 1; i++)
	{
		sPath << (*m_components)[i] << L'\\';
	}
	sPath << (*m_components)[m_components->size() - 1];
	return sPath;
}

UTF16 FIItem::getBasename()
{
	if (m_components->size() == 0)
		return L"";
	else
		return (*m_components)[m_components->size() - 1];
}

UTF16 FIItem::getExt()
{
	return m_sExt;
}

UTF16 FIItem::getExtShortString()
{
	return m_sExtShort;
}

UTF16 FIItem::getExtLongString()
{
	return m_sExtLong;
}

UInt64 FIItem::getSize()
{
	return m_iFileSize;
}

TimePoint FIItem::getCreated()
{
	return m_created;
}

TimePoint FIItem::getModified()
{
	return m_modified;
}

FIItem::HComponents FIItem::getComponents()
{
	return m_components;
}

UTF16 FIItem::getSizeShortString()
{
	return FileSize(m_iFileSize).getShortString();
}

UTF16 FIItem::getSizeLongString()
{
	return FileSize(m_iFileSize).getLongString();
}

UTF16 FIItem::getCreatedShortString()
{
	return m_created.toString("Y-M-D");
}

UTF16 FIItem::getCreatedLongString()
{
	return m_created.toString("Y-M-D h:m:s");
}

UTF16 FIItem::getModifiedShortString()
{
	return m_modified.toString("Y-M-D");
}

UTF16 FIItem::getModifiedLongString()
{
	return m_modified.toString("Y-M-D h:m:s");
}
