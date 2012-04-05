// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/InputStream.h"
#include "Stream/OutputStream.h"

#include "Stream/JSONObject.h"
#include "Stream/JSONArray.h"
/* forward decl */ class JSONParser;

class JSON
{
public:
	static JSON parse(const UTF8&);
	static JSON parse(HInputStream&);

	void generate(Error&, const HOutputStream&) const;
	UTF8 generate(Error&) const;

	inline JSON(/* null */);
	inline explicit JSON(bool);
	inline explicit JSON(SInt8);
	inline explicit JSON(SInt16);
	inline explicit JSON(SInt32);
	inline explicit JSON(SInt64);
	inline explicit JSON(UInt8);
	inline explicit JSON(UInt16);
	inline explicit JSON(UInt32);
	inline explicit JSON(UInt64);
	inline explicit JSON(Real32);
	inline explicit JSON(Real64);
	inline explicit JSON(const Chr8*);
	inline explicit JSON(const UTF8&);
	inline explicit JSON(const Chr16*);
	inline explicit JSON(const UTF16&);

	inline bool isNull() const;
	inline bool isSet() const;
	void setNull();

	bool as_bool() const;
	SInt64 as_integer() const;
	Real64 as_real() const;
	UTF16 as_string() const;
	void as_string(Error&, const HOutputStream&) const;
	JSON as_array() const;
	JSON as_object() const;

	Size size() const;

	JSON& operator[] (Size iArrayIndex);
	void push(const JSON&);
	JSON pop();

	JSON& operator[] (const UTF16& sKey);
	void del(const UTF16& sKey);

	static SInt64 comparef(const JSON& a, const JSON& b);

	inline bool operator== (const JSON& b) const
	{
		return JSON::comparef((*this), b) == 0;
	}

	inline bool operator< (const JSON& b) const
	{
		return JSON::comparef((*this), b) < 0;
	}

	inline bool operator != (const JSON& b) const { return !((*this) == b); }
	inline bool operator >  (const JSON& b) const { return JSON(b) < (*this); }
	inline bool operator >= (const JSON& b) const { return !((*this) < b); }
	inline bool operator <= (const JSON& b) const { return JSON(b) >= (*this); }

private:

	enum EType
	{
		e_null,
		e_bool,
		e_integer,
		e_real,
		e_string,
		e_array,
		e_object
	};

	inline explicit JSON(EType);

	EType m_type;

	bool m_bool;
	SInt64 m_integer;
	Real64 m_real;
	UTF16 m_string;
	JSONArray m_array;
	JSONObject m_object;

	static JSON stringToNumber(const UTF16& sString);
	static JSON arrayToNumber(const JSONArray& arr);
	static JSON objectToNumber(const JSONObject& obj);

	void convertToArray();
	void convertToObject();

	static SInt64 cmp_null(const JSON& a, const JSON& b);
	static SInt64 cmp_bool(const JSON& a, const JSON& b);
	static SInt64 cmp_integer(const JSON& a, const JSON& b);
	static SInt64 cmp_real(const JSON& a, const JSON& b);
	static SInt64 cmp_string(const JSON& a, const JSON& b);
	static SInt64 cmp_array(const JSON& a, const JSON& b);
	static SInt64 cmp_object(const JSON& a, const JSON& b);

	friend class JSONArray;
	friend class JSONObject;
	friend class JSONParser;
};

inline JSON::JSON() : m_type(e_null) {}

inline JSON::JSON(bool x) : m_type(e_bool), m_bool(x) {};

inline JSON::JSON(SInt8  x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(SInt16 x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(SInt32 x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(SInt64 x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(UInt8  x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(UInt16 x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(UInt32 x) : m_type(e_integer), m_integer(x) {};
inline JSON::JSON(UInt64 x) : m_type(e_integer), m_integer(x) {};

inline JSON::JSON(Real32 x) : m_type(e_real), m_real(x) {};
inline JSON::JSON(Real64 x) : m_type(e_real), m_real(x) {};

inline JSON::JSON(const Chr8* x) : m_type(e_string), m_string(UTF16(UTF8(x))) {};
inline JSON::JSON(const UTF8& x) : m_type(e_string), m_string(UTF16(x)) {};
inline JSON::JSON(const Chr16* x) : m_type(e_string), m_string(x) {};
inline JSON::JSON(const UTF16& x) : m_type(e_string), m_string(x) {};

inline JSON::JSON(EType e_type) : m_type(e_type) {}

inline bool JSON::isNull() const { return m_type == e_null; }
inline bool JSON::isSet() const { return m_type != e_null; }

