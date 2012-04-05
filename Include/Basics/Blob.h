// (C) 2009 Tomazos-Jager Software Labs
#pragma once
#include "global.h"

#include "Basics/Handle.h"
#include "Basics/Handleable.h"
#include "Basics/DefaultHasher.h"
#include "Basics/DefaultComparator.h"

class Blob
{
public:
	static Blob own(void* p, Size iNumBytes);

	inline Blob();
	Blob(const void* p, Size iNumBytes);
	inline Blob(const Blob&);

	inline Blob& operator=(const Blob&);

	inline Size length() const;
	inline bool empty() const { return 0 == length(); }

	inline const void* ptr() const;

	template <class T>
	T& idx(Size i) { return ((T*)ptr())[i]; }

	bool operator==(const Blob&) const;
	inline bool operator<(const Blob&) const;

	inline bool operator!=(const Blob&) const;
	inline bool operator>(const Blob&) const;
	inline bool operator<=(const Blob&) const;
	inline bool operator>=(const Blob&) const;

	static UInt64 hashf(const Blob& x);
	static SInt64 comparef(const Blob& a, const Blob& b);


private:
	void setEmpty();

	class Core : public Handleable
	{
	public:
		Size m_iLength;
		
		union
		{
			void* m_pContent;
			Chr8* m_pChr8Content;
			Chr16* m_pChr16Content;
		};

		~Core();
	};

	Blob(const Handle<Core>&);

	Handle<Core> m_core;

};

DeclHashable(Blob);
DeclComparable(Blob);

inline bool
Blob::operator<(const Blob& that) const
{
	return comparef(*this, that) < 0;
}

inline bool
Blob::operator!=(const Blob& that) const
{
	return !((*this) == that);
}

inline bool
Blob::operator>(const Blob& that) const
{
	return that < (*this);
}

inline bool
Blob::operator<=(const Blob& that) const
{
	return !((*this) > that);
}

inline bool
Blob::operator>=(const Blob& that) const
{
	return !((*this) < that);
}

inline Blob::Blob(const Handle<Core>& core)
	: m_core(core)
{

}

inline Blob::Blob()
{
	setEmpty();
}

inline Blob::Blob(const Blob& that)
	: m_core(that.m_core)
{

}

inline Blob&
Blob::operator=(const Blob& that)
{
	m_core = that.m_core;
	return (*this);
}

inline const void* Blob::ptr() const
{
	return (void*) m_core->m_pContent;
}

inline Size
Blob::length() const
{
	return m_core->m_iLength;
}
