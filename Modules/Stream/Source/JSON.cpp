// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/JSON.h"

#include "Basics.h"

#include "Stream/BufferedOutputStream.h"
#include "Stream/MemoryOutputStream.h"
#include "Stream/TCP.h"

#include <cmath>

#include "Stream/BlobInputStream.h"

#include "JSONParser.h"

#include "Runtime.h"

const Chr8* g_sNullUTF8("null");
const Chr8* g_sTrueUTF8("true");
const Chr8* g_sFalseUTF8("false");
const Chr16* g_sNullUTF16(L"null");
const Chr16* g_sTrueUTF16(L"true");
const Chr16* g_sFalseUTF16(L"false");

void JSON::setNull()
{
	(*this) = JSON();
}

class StringToNumberConverter
{
public:
	StringToNumberConverter(const Chr16* p) : m_p(p) {}
	const Chr16* m_p;
	void skipWhitespace() { while (Character::isWhitespace(*m_p)) m_p++; }
	Chr16 peek() { skipWhitespace(); return *m_p; }
	Chr16 pop() { skipWhitespace(); return *(m_p++); }
	bool tryParse(Chr16 c) { if (c == peek()) { pop(); return true; } else return false; }

	JSON parse()
	{
		bool bPositive = (!tryParse(L'-'));
		
		if (bPositive)
			tryParse(L'+');

		SInt64 iBase;
		UInt8 iFirstDigit;
		if (Character::decDigit(peek(), iFirstDigit) == Character::eCodingFailure)
			return JSON(1);
		iBase = iFirstDigit;

		pop();
		if (iBase != 0)
		{
			UInt8 iNextDigit;
			while (Character::decDigit(peek(), iNextDigit) == Character::eCodingSuccess)
			{
				pop();
				iBase *= 10;
				iBase += iNextDigit;
			}
		}
		Real64 fBase = Real64(iBase);
		bool bPoint = tryParse(L'.');
		if (bPoint)
		{
			Real64 fFactor = 0.1;
			UInt8 iNextDigit;
			while (Character::decDigit(peek(), iNextDigit) == Character::eCodingSuccess)
			{
				pop();
				fBase += (fFactor * iNextDigit);
				fFactor /= 10;
			}
		}
		bool bExponent = (tryParse(L'e') || tryParse(L'E'));
		if (bExponent)
		{
			bool bPositiveExponent = (!tryParse(L'-'));
			
			if (bPositiveExponent)
				tryParse(L'+');

			SInt64 iExponent;
			UInt8 iFirstDigit;
			if (Character::decDigit(peek(), iFirstDigit) == Character::eCodingSuccess)
			{
				iExponent = iFirstDigit;
				pop();

				UInt8 iNextDigit;
				while (Character::decDigit(peek(), iNextDigit) == Character::eCodingSuccess)
				{
					pop();
					iExponent *= 10;
					iExponent += iNextDigit;
				}

				if (bPositiveExponent)
					fBase *= std::pow(Real64(10),Real64(iExponent));
				else
					fBase *= std::pow(Real64(10),Real64(-iExponent));
			}
			else
				bExponent = false;
		}
		if (bPoint || bExponent)
		{
			if (bPositive)
				return JSON(fBase);
			else
				return JSON(-fBase);
		}
		else
		{
			if (bPositive)
				return JSON(iBase);
			else
				return JSON(-iBase);
		}
	}
};

JSON JSON::stringToNumber(const UTF16& sString)
{
	if (sString.length() == 0)
		return JSON(0);

	return StringToNumberConverter(sString.ptr()).parse();
}

JSON JSON::arrayToNumber(const JSONArray& arr)
{
	return arr.as_number();
};

JSON JSON::objectToNumber(const JSONObject& obj)
{
	return obj.as_number();
}

bool JSON::as_bool() const
{
	switch (m_type)
	{
	case e_null: return false;
	case e_bool: return m_bool;
	case e_integer: return as_integer() != 0;
	case e_real: return as_real() != Real64(0);
	case e_string: return stringToNumber(m_string).as_bool();
	case e_array: return arrayToNumber(m_array).as_bool();
	case e_object: return objectToNumber(m_object).as_bool();
	}
	return 0;
}

SInt64 JSON::as_integer() const
{
	switch (m_type)
	{
	case e_null: return 0;
	case e_bool: return (m_bool ? 1 : 0);
	case e_integer: return m_integer;
	case e_real: return SInt64(m_real);
	case e_string: return stringToNumber(m_string).as_integer();
	case e_array: return arrayToNumber(m_array).as_integer();
	case e_object: return objectToNumber(m_object).as_integer();
	}
	return 0;
}

Real64 JSON::as_real() const
{
	switch (m_type)
	{
	case e_null: return 0;
	case e_bool: return (m_bool ? 1 : 0);
	case e_integer: return Real64(m_integer);
	case e_real: return m_real;
	case e_string: return stringToNumber(m_string).as_real();
	case e_array: return arrayToNumber(m_array).as_real();
	case e_object: return objectToNumber(m_object).as_real();
	}
	return 0;
}

UTF16 JSON::as_string() const
{
	if (m_type == e_string)
		return m_string;

	Buffer buf(0);

	{
		Error error;
		HOutputStream out = NewObject(BufferedOutputStream, NewObject(MemoryOutputStream,buf), 512);
		as_string(error, out);

		Check(!error);

		out->write(error, L"", 2);

		Check(!error);
	}

	return UTF16(Blob(buf));
}

void JSON::as_string(Error& error, const HOutputStream& out) const
{
	switch (m_type)
	{
	case e_null:    out->twrite(error, UTF16(L"")); break;
	case e_bool:    out->twrite(error, m_bool ? UTF16(g_sTrueUTF16) : UTF16(g_sFalseUTF16)); break;
	case e_integer: out->twrite(error, UTF16::format(L"%lld", m_integer)); break;
	case e_real:    out->twrite(error, UTF16::format(L"%e", m_real)); break;
	case e_string:  out->twrite(error, m_string); break;
	case e_array:   m_array.as_string(error, out); break;
	case e_object:  m_object.as_string(error, out); break;
	}
};

Size JSON::size() const
{
	switch (m_type)
	{
	case e_null: return 0;
	case e_array: return m_array.size();
	case e_object: return m_object.size();
	default: return 1;
	}
}

JSON& JSON::operator[] (Size iArrayIndex)
{
	convertToArray();

	return m_array[iArrayIndex];
}

void JSON::push(const JSON& item)
{
	convertToArray();

	return m_array.push(item);
}

JSON JSON::pop()
{
	convertToArray();

	return m_array.pop();
}

JSON JSON::as_array() const
{
	switch (m_type)
	{
	case e_null: return JSON(e_array);
	case e_object: return m_object.as_array();
	case e_array: return *this;
	default:
		{
			JSON ret = JSON(e_array);
			ret.push(*this);
			return ret;
		}
	}
}

void JSON::convertToArray()
{
	if (m_type != e_array)
	{
		JSON arr = as_array();
		(*this) = arr;
	}
}

const Chr16* g_sScalarKey(L"scalar");

JSON JSON::as_object() const
{
	switch (m_type)
	{
	case e_null: return JSON(e_object);
	case e_array: return m_array.as_object();
	case e_object: return *this;
	default:
		{
			JSON ret = JSON(e_object);
			ret[UTF16(g_sScalarKey)] = (*this);
			return ret;
		}
	}
}

void JSON::convertToObject()
{
	if (m_type != e_object)
	{
		JSON obj = as_object();
		(*this) = obj;
	}
}

JSON& JSON::operator[] (const UTF16& sKey)
{
	convertToObject();

	return m_object[sKey];
}

void JSON::del(const UTF16& sKey)
{
	convertToObject();

	m_object.del(sKey);
}

SInt64 JSON::comparef(const JSON& a, const JSON& b)
{
	switch (a.m_type)
	{
	case e_null: 
		switch (b.m_type)
		{
			case e_null: return cmp_null(a,b);
			case e_bool: return cmp_bool(a,b);
			case e_integer: return cmp_integer(a,b);
			case e_real: return cmp_real(a,b);
			case e_string: return cmp_string(a,b);
			case e_array: return cmp_array(a,b);
			case e_object: return cmp_object(a,b);
		}

	case e_bool:
		switch (b.m_type)
		{
			case e_null: return cmp_bool(a,b);
			case e_bool: return cmp_bool(a,b);
			case e_integer: return cmp_integer(a,b);
			case e_real: return cmp_real(a,b);
			case e_string: return cmp_string(a,b);
			case e_array: return cmp_array(a,b);
			case e_object: return cmp_object(a,b);
		}

	case e_integer:
		switch (b.m_type)
		{
			case e_null: return cmp_integer(a,b);
			case e_bool: return cmp_integer(a,b);
			case e_integer: return cmp_integer(a,b);
			case e_real: return cmp_real(a,b);
			case e_string: return cmp_string(a,b);
			case e_array: return cmp_array(a,b);
			case e_object: return cmp_object(a,b);
		}

	case e_real:
		switch (b.m_type)
		{
			case e_null: return cmp_real(a,b);
			case e_bool: return cmp_real(a,b);
			case e_integer: return cmp_real(a,b);
			case e_real: return cmp_real(a,b);
			case e_string: return cmp_string(a,b);
			case e_array: return cmp_array(a,b);
			case e_object: return cmp_object(a,b);
		}

	case e_string:
		switch (b.m_type)
		{
			case e_null: return cmp_string(a,b);
			case e_bool: return cmp_string(a,b);
			case e_integer: return cmp_string(a,b);
			case e_real: return cmp_string(a,b);
			case e_string: return cmp_string(a,b);
			case e_array: return cmp_array(a,b);
			case e_object: return cmp_object(a,b);
		}

	case e_array:
		switch (b.m_type)
		{
			case e_null: return cmp_array(a,b);
			case e_bool: return cmp_array(a,b);
			case e_integer: return cmp_array(a,b);
			case e_real: return cmp_array(a,b);
			case e_string: return cmp_array(a,b);
			case e_array: return cmp_array(a,b);
			case e_object: return cmp_object(a,b);
		}

	case e_object:
		switch (b.m_type)
		{
			case e_null: return cmp_object(a,b);
			case e_bool: return cmp_object(a,b);
			case e_integer: return cmp_object(a,b);
			case e_real: return cmp_object(a,b);
			case e_string: return cmp_object(a,b);
			case e_array: return cmp_object(a,b);
			case e_object: return cmp_object(a,b);
		}
	}

	Check();
	return -1;
}

SInt64 JSON::cmp_null(const JSON& a, const JSON& b)
{
	return 0;
}

SInt64 JSON::cmp_bool(const JSON& a, const JSON& b)
{
	return (a.as_bool() ? 1 : 0) - (b.as_bool() ? 1 : 0);
}

SInt64 JSON::cmp_integer(const JSON& a, const JSON& b)
{
	SInt64 iA = a.as_integer();
	SInt64 iB = b.as_integer();
	
	if (iA == iB)
		return 0;

	if (iA > iB)
		return 1;
	
	return -1;
}

SInt64 JSON::cmp_real(const JSON& a, const JSON& b)
{
	Real64 iA = a.as_real();
	Real64 iB = b.as_real();

	if (iA == iB)
		return 0;

	if (iA > iB)
		return 1;

	return -1;
}

SInt64 JSON::cmp_string(const JSON& a, const JSON& b)
{
	return UTF16::comparef(a.as_string(), b.as_string());
}

SInt64 JSON::cmp_array(const JSON& a, const JSON& b)
{
	JSON arra = a.as_array();
	JSON arrb = b.as_array();

	return JSONArray::comparef(arra.m_array, arrb.m_array);
}

SInt64 JSON::cmp_object(const JSON& a, const JSON& b)
{
	return cmp_array(a,b);
}

JSON JSON::parse(const UTF8& s)
{
	HInputStream in = NewObject(BlobInputStream, s);
	return parse(in);
}

JSON JSON::parse(HInputStream& ins)
{
	return JSONParser::parseValue(ins);
}

static void generateJSONString(Error& error, const HOutputStream& out, const UTF8& s)
{
	UTF16Buffer sb(2 + s.length() + 1);

	sb << '"';

	if (error)
		return;

	for (const Chr8* p = s.ptr(); (*p) != '\0'; p++)
	{
		Chr8 c = *p;

		switch (c)
		{
		case '\"' : sb << '\\' << '\"'; break;
		case '\\' : sb << '\\' << '\\'; break;
		case '/' :  sb << '\\' << '/'; break;
		case '\b' : sb << '\\' << 'b'; break;
		case '\f' : sb << '\\' << 'f'; break;
		case '\n' : sb << '\\' << 'n'; break;
		case '\r' : sb << '\\' << 'r'; break;
		case '\t' : sb << '\\' << 't'; break;
		default:
			if (c >= 0x20)
			{
				sb << c;
			}
			else
			{
				sb << UTF8("\\u00") << UTF8::format("%.2X",UInt32(c));
			}
		break;
		}
	}

	sb << '"';

	out->twrite(error, UTF8(sb));
};

void JSON::generate(Error& error, const HOutputStream& out) const
{
	switch (m_type)
	{
	case e_null: out->twrite(error, UTF8(g_sNullUTF8)); break;
	case e_bool: out->twrite(error, m_bool ? UTF8(g_sTrueUTF8) : UTF8(g_sFalseUTF8)); break;
	case e_integer: out->twrite(error, UTF8::format("%lld", m_integer)); break;
	case e_real: out->twrite(error, UTF8::format("%e", m_real)); break;
	case e_string: generateJSONString(error, out, UTF8(m_string)); break;
	case e_array: m_array.generate(error, out); break;
	case e_object: m_object.generate(error, out); break;
	}
}

UTF8 JSON::generate(Error& error) const
{
	Buffer buf(0);
	HOutputStream out = NewObject(MemoryOutputStream, buf);
	generate(error, out);
	out->twrite(error,'\0');
	return UTF8(Blob(buf));
}
