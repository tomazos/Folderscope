// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/JSONArray.h"

#include "Stream/JSON.h"

#include "Runtime.h"

#include <vector>

Size JSONArray::size() const
{
	return m_items.size();
}

JSON& JSONArray::operator[](Size iIndex)
{
	if (iIndex >= 0)
	{
		if (iIndex >= size())
			m_items.resize(iIndex+1);

		return m_items[iIndex];
	}
	else
	{
		iIndex = size() + iIndex;
		if (iIndex < 0 || iIndex >= size())
			iIndex = 0;

		return m_items[iIndex];
	}
}

void JSONArray::push(const JSON& item)
{
	m_items.push_back(item);
}

JSON JSONArray::pop()
{
	if (size() > 0)
	{
		JSON j = m_items.back();
		m_items.pop_back();
		return j;
	}
	else
		return JSON();
}

void JSONArray::generate(Error& error, const HOutputStream& out) const
{
	out->twrite(error, '[');

	if (error)
		return;

	for (int i = 0; i < size(); i++)
	{
		m_items[i].generate(error, out);

		if (error)
			return;

		if (i != size() - 1)
		{
			out->twrite(error,UTF8(", "));

			if (error)
				return;
		}
	}

	out->twrite(error, UTF8("]"));

	if (error)
		return;
}

void JSONArray::as_string(Error& error, const HOutputStream& out) const
{
	out->twrite(error, UTF16(L"("));

	if (error)
		return;

	for (int i = 0; i < size(); i++)
	{
		m_items[i].as_string(error, out);

		if (error)
			return;

		if (i != size() - 1)
		{
			out->twrite(error, UTF16(L" "));
			if (error)
				return;
		}
	}

	out->twrite(error, UTF16(L")"));
}

JSON JSONArray::as_number() const
{
	if (size() == 1)
		return m_items[0];
	else
		return JSON(size());
}

JSON JSONArray::as_object() const
{
	JSON obj;

	for (int i = 0; i <= size() - 2; i+=2)
	{
		JSON k = m_items[i];
		JSON v = m_items[i+1];
		obj[k.as_string()] = v;
	}

	return obj;
}

SInt64 JSONArray::comparef(const JSONArray& in_a, const JSONArray& in_b)
{
	JSONArray a(in_a);
	JSONArray b(in_b);

	int iLeastSize = ( (a.size() < b.size()) ? a.size() : b.size() );
	for (int i = 0; i < iLeastSize; i++)
	{
		SInt64 iCmp = JSON::comparef(a[i], b[i]);
		if (iCmp != 0)
		{
			return iCmp;
		}
	}

	return a.size() - b.size();
}
