#include "vector.h"
#include "../ds_routines.h"
#include <cstdlib>
#include <cstring>

namespace structures {

	Vector::Vector(size_t size) :
		memory_(calloc(size, 1)),
		size_(size)
	{
	}

	Vector::Vector(const Vector& other) :
		Vector(other.size_)
	{
		//vdaka calloc je 100% iste ze sa nebudu prekryvat => pouzijem memcpy
		memory_ = memcpy(memory_, other.memory_, size_);
	}

	Vector::~Vector()
	{
		free(memory_);
		memory_ = nullptr;
		size_ = 0;
	}

	Structure* Vector::clone() const
	{
		return new Vector(*this);
	}

	size_t Vector::size() const
	{
		return size_;
	}

	Structure & Vector::operator=(const Structure & other)
	{
		if (this != &other)
		{
			*this = dynamic_cast<const Vector&>(other);
		}
		return *this;
	}

	Vector& Vector::operator=(const Vector& other)
	{
		if (this != &other)
		{
			size_ = other.size_;
			memory_ = realloc(memory_, size_);
			memory_ = memcpy(memory_, other.memory_, size_);	//kam, odkial, kolko
		}
		return *this;
	}

	bool Vector::operator==(const Vector& other) const
	{
		return size_ == other.size_ && memcmp(memory_, other.memory_, size_) == 0;
	}

	//vracia mi odkazom; pole[3] = 10;
	byte& Vector::operator[](const int index)
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index in Vector!");
		return *(reinterpret_cast<byte*>(memory_) + index);
	}

	//vracia mi to ako hodnotu; int a = pole[3]
	byte Vector::operator[](const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index in Vector!");
		return *(reinterpret_cast<byte*>(memory_) + index);
	}

	byte& Vector::readBytes(const int index, const int count, byte& dest)
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index in Vector!");
		DSRoutines::rangeCheckExcept((index + count), size_ + 1, "Invalid end-index in Vector!");			//+1 aby posledny prvok zahrnul aj uplny koniec vektora 


		memcpy(&dest, getBytePointer(index), count);
		return dest;
	}

	void Vector::copy(const Vector& src, const int srcStartIndex, Vector& dest, const int destStartIndex, const int length)
	{
		if (length != 0)
		{
			DSRoutines::rangeCheckExcept(srcStartIndex, src.size_, "Invalid index in Source Vector!");
			DSRoutines::rangeCheckExcept((srcStartIndex + length), src.size_ + 1, "Invalid end-index in Source Vector!");
			DSRoutines::rangeCheckExcept(destStartIndex, dest.size_, "Invalid index in Destination Vector!");
			DSRoutines::rangeCheckExcept((destStartIndex + length), dest.size_ + 1, "Invalid end-index in Destination Vector!");

			if (src == dest && (srcStartIndex + length <= destStartIndex || destStartIndex + length <= srcStartIndex))
			{
				memmove(src.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
			}
			else
			{
				memcpy(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
			}
		}

	}

	byte* Vector::getBytePointer(const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index in Vector!");
		return (reinterpret_cast<byte*>(memory_) + index);
	}

}