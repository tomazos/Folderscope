// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "JSONParser.h"

#include "Stream/BufferedInputStream.h"

#include <cmath>

JSON JSONParser::parseValue(HInputStream in)
{
	try
	{
		JSONParser parser(in);
		return parser.value_();
	}
	catch (JSONParser::JError&)
	{
		return JSON();
	}
}

JSONParser::JSONParser(HInputStream in)
	: m_in(NewObject(BufferedInputStream, in, 512))
	, m_c(0)
{
	if (m_in->read(m_error, &m_c, 1) != 1)
	{
		m_in.setNull();
		throw JError();
	}

	if (m_error)
	{
		m_in.setNull();
		throw JError();
	}
}

JSON JSONParser::value_()
{
	JSON ret;
	ws_();
	switch (peek())
	{
	case 'f': ret= false_(); break;
	case 'n': ret= null_(); break;
	case 't': ret= true_(); break;
	case '{': ret= object_(); break;
	case '[': ret= array_(); break;
	case '"': ret= JSON(string_()); break;
	default:  ret= number_(); break;
	}
	ws_();
	return ret;
}

void JSONParser::ws_()
{
	while(Character::isWhitespace(peek()))
		pop();
}

JSON JSONParser::false_()
{
	pop("false");
	return JSON(false);
}

void JSONParser::pop(const Chr8* in_s)
{
	for (const Chr8* s = in_s; (*s) != '\0'; s++)
	{
		pop(*s);
	}
}

void JSONParser::pop(Chr8 c)
{
	if (peek() == c)
		pop();
	else
		throw JError();
}

Chr8 m_c;

Chr8 JSONParser::peek()
{
	return m_c;
}

void JSONParser::pop()
{
	bool bReadFailed = (m_in->read(m_error, &m_c, 1) != 1);

	if (m_error || bReadFailed)
	{
		m_c = 0;
	}
}

JSON JSONParser::null_()
{
	pop("null");
	return JSON();
}

JSON JSONParser::true_()
{
	pop("true");
	return JSON(true);
}

JSON JSONParser::object_()
{
	begin_object_();
	if (peek() == '}')
	{
		end_object_();
		return JSON(JSON::e_object);
	}
	JSON obj = JSON(JSON::e_object);
	member_(obj);
	while (peek() != '}')
	{
		value_separator_();
		member_(obj);
	}
	end_object_();
	return obj;
}

void JSONParser::begin_object_()
{
	ws_();
	pop('{');
	ws_();
}

void JSONParser::end_object_()
{
	ws_();
	pop('}');
	ws_();
}

void JSONParser::member_(JSON& obj)
{
	UTF16 sKey = string_();
	name_separator_();
	JSON sValue = value_();
	obj[sKey] = sValue;
}

void JSONParser::name_separator_()
{
	ws_();
	pop(':');
	ws_();
}

void JSONParser::value_separator_()
{
	ws_();
	pop(',');
	ws_();
}

JSON JSONParser::array_()
{
	begin_array_();
	if (peek() == ']')
	{
		end_array_();
		return JSON(JSON::e_array);
	}
	JSON arr = JSON(JSON::e_array);
	arr.push(value_());
	while (peek() != ']')
	{
		value_separator_();
		arr.push(value_());
	}
	end_array_();
	return arr;
}

void JSONParser::begin_array_()
{
	ws_();
	pop('[');
	ws_();
}

void JSONParser::end_array_()
{
	ws_();
	pop(']');
	ws_();
}

UTF16 JSONParser::string_()
{
	Array<Chr16> arr;

	quotation_mark_();

	while (peek() != '"')
	{
		char_(arr);
	};

	quotation_mark_();

	arr.push(L'\0');
	return UTF16(arr.ptr());
}

void JSONParser::quotation_mark_()
{
	pop('"');
}

void JSONParser::char_(Array<Chr16>& arr)
{
	if (peek() == '\\')
		escaped_(arr);
	else
		unescaped_(arr);
}

void JSONParser::escaped_(Array<Chr16>& arr)
{
	pop('\\');
	switch (peek())
	{
	case '"' : pop('"'); arr.push(L'"'); return;
	case '\\' : pop('\\'); arr.push(L'\\'); return;
	case '/' : pop('/'); arr.push(L'/'); return;
	case 'b' : pop('b'); arr.push(L'\b'); return;
	case 'f' : pop('f'); arr.push(L'\f'); return;
	case 'n' : pop('n'); arr.push(L'\n'); return;
	case 'r' : pop('r'); arr.push(L'\r'); return;
	case 't' : pop('t'); arr.push(L'\t'); return;
	case 'u' : pop('u'); hexdig_4_(arr); return;
	default: throw JError();
	}
}

void JSONParser::hexdig_4_(Array<Chr16>& arr)
{
	UInt16 c = 0;
	for (int i = 0; i < 4; i++)
	{
		c <<= 4;
		c |= UInt16(hexdig_());
	}
	arr.push(Chr16(c));
}

void JSONParser::unescaped_(Array<Chr16>& arr_utf16)
{
	Chr8 c = peek();
	if (c <= Chr8(0x7F))
	{
		if (c < Chr8(0x20) || c == Chr8(0x22) || c == Chr8(0x5C))
			throw JError();
		
		pop();
		arr_utf16.push(Chr16(c));
	}
	else
	{
		Array<Chr8> arr_utf8;
		arr_utf8.push(c);
		pop();
		while (peek() & 0x80)
		{
			arr_utf8.push(peek());
			pop();
		}

		arr_utf8.push('\0');
		UTF8 sUTF8 = UTF8(arr_utf8.ptr());
		UTF16 sUTF16;
		if (Unicode::UTF8_to_UTF16(sUTF8, sUTF16) == Character::eCodingFailure)
			throw JError();

		for (const Chr16* p = sUTF16.ptr(); (*p) != L'\0'; p++)
			arr_utf16.push(*p);
	}
}


UInt8 JSONParser::hexdig_()
{
	UInt8 c;
	
	if (Character::eCodingFailure == Character::hexDigit(peek(),c))
		throw JError();

	pop();
	return c;
}


JSON JSONParser::number_()
{
	NumberInfo info;

	if (peek() == '-')
	{
		minus_();
		info.m_sign = -1;
	}

	int_(info);

	if (peek() == '.')
		frac_(info);
	if (peek() == 'e' || peek() == 'E')
		exp_(info);

	switch (info.m_type)
	{
	case NumberInfo::eInt:
		if (info.m_sign == +1)
		{
			UInt64 i = info.m_int;
			if (i < 0x100ULL) return JSON(UInt8(i));
			if (i < 0x10000ULL) return JSON(UInt16(i));
			if (i < 0x100000000ULL) return JSON(UInt32(i));
			return JSON(i);
		}
		else
		{
			SInt64 i = -SInt64(info.m_int);
			if (i >= -0x80LL) return JSON(SInt8(i));
			if (i >= -0x8000LL) return JSON(SInt16(i));
			if (i >= -0x80000000LL) return JSON(SInt32(i));
			return JSON(i);
		}
	case NumberInfo::eReal:
		info.m_real *= info.m_sign;
		if (Real64(info.m_real) == Real64(Real32(info.m_real)))
			return JSON(Real32(info.m_real));
		else
			return JSON(Real64(info.m_real));
	}
	throw JError();
}

void JSONParser::minus_()
{
	pop('-');
}

void JSONParser::plus_()
{
	pop('+');
}

void JSONParser::zero_()
{
	pop('0');
}

void JSONParser::int_(NumberInfo& info)
{
	info.m_int = digit_();

	if (info.m_int != 0)
		while (peek_digit())
		{
			info.m_int *= 10;
			info.m_int += digit_();
		}
}

bool JSONParser::peek_digit()
{
	UInt8 dummy;
	return (Character::decDigit(peek(), dummy) == Character::eCodingSuccess);
}

UInt8 JSONParser::digit_()
{
	UInt8 iDigit;
	if (Character::decDigit(peek(), iDigit) != Character::eCodingSuccess)
		throw JError();

	pop();
	return iDigit;
}

void JSONParser::frac_(NumberInfo& info)
{
	pop('.');

	info.m_type = NumberInfo::eReal;

	Real64 fDiv = 0.1;

	info.m_real = Real64(info.m_int) + Real64(digit_()) * fDiv;

	while (peek_digit())
	{
		fDiv *= 0.1;
		info.m_real += fDiv * digit_();
	}
}

void JSONParser::exp_(NumberInfo& info)
{
	info.m_type = NumberInfo::eReal;
	SInt8 iSign = +1;
	e_();
	if (peek() == '-')
	{
		minus_();
		iSign = -1;
	}
	else if (peek() == '+')
	{
		plus_();
		iSign = +1;
	}

	Real64 fExp = digit_();
	while (peek_digit())
	{
		fExp *= 10;
		fExp += digit_();
	}

	info.m_real *= std::pow(Real64(10), Real64(fExp * iSign));
}

void JSONParser::e_()
{
	if (peek() == 'E' || peek() == 'e')
		pop();
	else
		throw JError();
}