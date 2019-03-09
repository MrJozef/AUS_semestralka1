#include "memory_routines.h"

namespace structures {
	byte & MemoryRoutines::byteSet(byte & B)
	{
		B = MAX_BYTE;
		return B;
	}
	byte & MemoryRoutines::byteReset(byte & B)
	{
		B = 0;
		return B;
	}
	byte & MemoryRoutines::byteXOR(byte & B)
	{
		B = B ^ MAX_BYTE;//exkluzivny bitovy sucet
		return B;
	}
	byte & MemoryRoutines::byteSHL(byte & B)
	{
		return B <<= 1;
	}
	byte & MemoryRoutines::byteSHR(byte & B)
	{
		return B >>= 1;
	}

	bool MemoryRoutines::byteNthBitGet(byte & B, int n)
	{
		return (B >> n) & 1;
	}

	byte & MemoryRoutines::byteNthBitTo0(byte & B, int n)
	{
		//~ je bytovy doplnok
		//1 posuniem na miesto n, potom spravim doplnok => na mieste n bude 0, vsade inde 1 a potom uz spravim logicky sucin
		return B &= ~(1 << n);
	}

	byte & MemoryRoutines::byteNthBitTo1(byte & B, int n)
	{
		//1 posuniem na n-tu poziciu, na vsetkych ostatnych poziciach budu 0 a spravim logicky sucet
		return B |= (1 << n);
	}

	byte & MemoryRoutines::byteNthBitToggle(byte & B, int n)
	{
		return B ^= (1 << n);
	}

	MemoryRoutines::MemoryRoutines()
	{
	}

}
