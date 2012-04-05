// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream/JSON.h"
#include "Stream/InputStream.h"

class JSONParser
{
public:

	static JSON parseValue(HInputStream in);

private:
	Error m_error;
	HInputStream m_in;

	JSONParser(HInputStream in);

	class JError {};

	class NumberInfo
	{
	public: 
		NumberInfo() : m_type(eInt), m_sign(+1), m_int(0), m_real(0) {}
		
		enum ENumberType
		{
			eInt,
			eReal
		};
		ENumberType m_type;
		SInt8 m_sign;
		UInt64 m_int;
		Real64 m_real;
	};


	JSON value_();
	void ws_();
	JSON false_();
	void pop(const Chr8* in_s);
	void pop(Chr8 c);
	Chr8 m_c;

	Chr8 peek();
	void pop();
	JSON null_();
	JSON true_();
	JSON object_();
	void begin_object_();
	void end_object_();
	void member_(JSON& obj);
	void name_separator_();
	void value_separator_();
	JSON array_();
	void begin_array_();
	void end_array_();
	UTF16 string_();
	void quotation_mark_();
	void char_(Array<Chr16>& arr);
	void escaped_(Array<Chr16>& arr);
	void hexdig_4_(Array<Chr16>& arr);
	void unescaped_(Array<Chr16>& arr_utf16);
	UInt8 hexdig_();

	JSON number_();
	void minus_();
	void plus_();
	void zero_();
	void int_(NumberInfo& info);

	bool peek_digit();
	UInt8 digit_();

	void frac_(NumberInfo& info);

	void exp_(NumberInfo& info);
	void e_();
};