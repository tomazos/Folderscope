// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
// This SHA2-512 implementation is based on an open source BSD-licensed version.
#include "Stream/SHA512.h"

#include "Basics.h"

static const Size iBlockLength = 128;
static const Size iDigestLength = 64;
static const Size iShortBlockLength	= iBlockLength - 16;

static inline UInt64 reverse(UInt64 n)
{
	n = (n >> 32) | (n << 32);
	n = ((n & 0xff00ff00ff00ff00ULL) >> 8) | ((n & 0x00ff00ff00ff00ffULL) << 8);
	return ((n & 0xffff0000ffff0000ULL) >> 16) | ((n & 0x0000ffff0000ffffULL) << 16);
}

static inline void addAs128(UInt64* p, UInt64 n)
{
	p[0] += n;

	if (p[0] < n)
		p[1]++;
}

#define R(b,x) 		((x) >> (b))
#define S64(b,x)	(((x) >> (b)) | ((x) << (64 - (b))))

#define Ch(x,y,z)	(((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define Sigma0_512(x)	(S64(28, (x)) ^ S64(34, (x)) ^ S64(39, (x)))
#define Sigma1_512(x)	(S64(14, (x)) ^ S64(18, (x)) ^ S64(41, (x)))
#define sigma0_512(x)	(S64( 1, (x)) ^ S64( 8, (x)) ^ R( 7,   (x)))
#define sigma1_512(x)	(S64(19, (x)) ^ S64(61, (x)) ^ R( 6,   (x)))

const static UInt64 K512[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
	0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
	0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
	0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
	0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
	0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
	0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
	0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
	0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
	0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
	0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
	0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
	0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
	0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
	0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
	0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
	0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
	0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
	0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
	0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
	0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

const static UInt64 sha512_initial_hash_value[8] = {
	0x6a09e667f3bcc908ULL,
	0xbb67ae8584caa73bULL,
	0x3c6ef372fe94f82bULL,
	0xa54ff53a5f1d36f1ULL,
	0x510e527fade682d1ULL,
	0x9b05688c2b3e6c1fULL,
	0x1f83d9abfb41bd6bULL,
	0x5be0cd19137e2179ULL
};

SHA512::SHA512()
{
	init();
}

void SHA512::init()
{
	Memory::copy(sha512_initial_hash_value, state, iDigestLength);
	Memory::zero(buffer, iBlockLength);
	bitcount[0] = bitcount[1] =  0;
}

void SHA512::transform(const UInt64* data)
{
	UInt64	a, b, c, d, e, f, g, h, s0, s1, T1, T2;
	UInt64* W512 = (UInt64*) buffer;

	int		j;

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	f = state[5];
	g = state[6];
	h = state[7];

	for (j = 0; j < 16; j++)
	{
		W512[j] = reverse(*data++);

		T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] + W512[j];
		T2 = Sigma0_512(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
	}

	for (; j < 80; j++)
	{
		s0 = W512[(j+1)&0x0f];
		s0 = sigma0_512(s0);
		s1 = W512[(j+14)&0x0f];
		s1 = sigma1_512(s1);

		T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] +
		     (W512[j&0x0f] += s1 + W512[(j+9)&0x0f] + s0);
		T2 = Sigma0_512(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
	}

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

void SHA512::write(const void * pBuf, Size iByteLen)
{
	const UInt8* data = (const UInt8*) pBuf;
	
	UInt32 len = UInt32(iByteLen);
	
	UInt32 freespace, usedspace;

	if (len == 0)
		return;

	usedspace = (bitcount[0] >> 3) % iBlockLength;
	if (usedspace > 0) {
		freespace = iBlockLength - usedspace;

		if (len >= freespace) {
			Memory::copy(data, &buffer[usedspace], freespace);
			addAs128(bitcount, freespace << 3);
			len -= freespace;
			data += freespace;
			transform((UInt64*)buffer);
		} else {
			Memory::copy(data, &buffer[usedspace], len);
			addAs128(bitcount, len << 3);

			usedspace = freespace = 0;
			return;
		}
	}
	
	while (len >= iBlockLength) {
		transform((UInt64*)data);
		addAs128(bitcount, iBlockLength << 3);
		len -= iBlockLength;
		data += iBlockLength;
	}

	if (len > 0) {
		Memory::copy(data, buffer, len);
		addAs128(bitcount, len << 3);
	}

	usedspace = freespace = 0;
}

void SHA512::last() {
	unsigned int usedspace = (bitcount[0] >> 3) % iBlockLength;

	bitcount[0] = reverse(bitcount[0]);
	bitcount[1] = reverse(bitcount[1]);

	if (usedspace > 0) {
		buffer[usedspace++] = 0x80;

		if (usedspace <= iShortBlockLength) {
			Memory::zero(&buffer[usedspace], iShortBlockLength - usedspace);
		} else {
			if (usedspace < iBlockLength)
				Memory::zero(&buffer[usedspace], iBlockLength - usedspace);

			transform((UInt64*)buffer);

			Memory::zero(buffer, iBlockLength - 2);
		}
	} else {
		Memory::zero(buffer, iShortBlockLength);

		buffer[0] = 0x80;
	}

	*(UInt64*)&buffer[iShortBlockLength] = bitcount[1];
	*(UInt64*)&buffer[iShortBlockLength+8] = bitcount[0];

	transform((UInt64*)buffer);
}

Blob SHA512::finish()
{
	UInt64	*d = (UInt64*) NewBytes(iDigestLength);

	last();

	for (int j = 0; j < 8; j++)
	{
		state[j] = reverse(state[j]);
		d[j] = state[j];
	}

	Blob b = Blob::own(d, iDigestLength);

	init();

	return b;
}
