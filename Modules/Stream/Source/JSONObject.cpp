// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/JSONObject.h"

#include "Stream/JSON.h"

JSONObject::JSONObject()
	: m_core(NewObject(Core))
{

}

JSONObject::JSONObject(const JSONObject& that)
	: m_core(that.m_core)
{}

Size JSONObject::size() const
{
	return m_core->m_tree.size();
}

JSON& JSONObject::operator[](const UTF16& sKey)
{
	return m_core->m_tree.viv(sKey);
}

void JSONObject::del(const UTF16& sKey)
{
	if (m_core->m_tree.has(sKey))
		m_core->m_tree.del(sKey);
}

void JSONObject::generate(Error& error, const HOutputStream& out) const
{
	out->twrite(error, Chr8('{'));

	if (error)
		return;

	if (size() > 0)
	{
		bool bFirst = true;

		TreeMap<UTF16, JSON>::Iterator it(m_core->m_tree);
		UTF16 sKey;
		JSON value;
		while (it(sKey, value))
		{
			if (bFirst)
				bFirst = false;
			else
			{
				out->twrite(error, UTF8(", "));
				if (error)
					return;
			}

			JSON(sKey).generate(error, out);

			if (error)
				return;

			out->twrite(error, UTF8(": "));

			if (error)
				return;

			value.generate(error, out);

			if (error)
				return;
		}
	}
	out->twrite(error, Chr8('}'));

	if (error)
		return;
}

void JSONObject::as_string(Error& error, const HOutputStream& out) const
{
	out->twrite(error, L'(');
	if (error) return;

	if (size() > 0)
	{
		bool bFirst = true;

		TreeMap<UTF16, JSON>::Iterator it(m_core->m_tree);
		UTF16 sKey;
		JSON value;
		while (it(sKey, value))
		{
			if (bFirst)
				bFirst = false;
			else
			{
				out->twrite(error, UTF16(L"; "));
				if (error)
					return;
			}

			out->twrite(error, UTF16(sKey));
			if (error)
				return;

			out->twrite(error, UTF16(L" = "));

			if (error)
				return;

			value.as_string(error, out);

			if (error)
				return;
		}
	}

	out->twrite(error, L')');

	if (error)
		return;
}

JSON JSONObject::as_array() const
{
	JSON result;
	TreeMap<UTF16, JSON>::Iterator it(m_core->m_tree);
	UTF16 sKey;
	JSON value;
	while (it(sKey, value))
	{
		result.push(JSON(sKey));
		result.push(value);
	}
	return result;
}

JSON JSONObject::as_number() const
{
	return JSON(size());
}