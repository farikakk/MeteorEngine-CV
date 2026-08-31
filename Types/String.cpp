/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#include "String.h"
#include "StringView.h"
#include <Logging/Log.h>
#include <Memory/MemoryHandler.h>

#include <cstdarg>
#include <cstdio>

#pragma warning(disable : 26495)
#pragma warning(disable : 5082) // second argument to 'va_start' is not the last named parameter

LOG_ADDCATEGORY(StringSet);

// BE ADVISED!	Allocate/Deallocate asks for bytes, well char size is 1 byte everywhere I think?

constexpr String::String()
	: internalBuffers()
{

}

String::~String() noexcept
{
	if (!SitsOnStack())
	{
		GetMemoryManager()->Deallocate(internalBuffers.heapBuffer.ptr);
	}
}

String::String(const char* Input)
{
	char* target = GetRecommendedBufferBySize(Count(Input));
	memcpy(target, Input, Count(Input));
}

String::String(String&& other) noexcept
{
	internalBuffers = other.internalBuffers;

	if (!other.SitsOnStack())
	{
		other.internalBuffers.heapBuffer.capacity = 0;
		other.internalBuffers.heapBuffer.length = 0;
		other.internalBuffers.heapBuffer.ptr = nullptr;
	}
	else
	{
		other.internalBuffers.stackBuffer.length = 0;
		other.internalBuffers.stackBuffer.ptr[0] = '\0';
	}
}

String::String(const String& other)
{
	if (!other.SitsOnStack())
	{
		internalBuffers.heapBuffer.capacity = other.internalBuffers.heapBuffer.capacity;
		internalBuffers.heapBuffer.length = other.internalBuffers.heapBuffer.length;

		u32 realCap = internalBuffers.heapBuffer.capacity & 0x7FFFFFFF;
		internalBuffers.heapBuffer.ptr = GetMemoryManager()->Allocate<char>(realCap);

		memset(internalBuffers.heapBuffer.ptr, 0, realCap);
		memcpy(internalBuffers.heapBuffer.ptr, other.internalBuffers.heapBuffer.ptr, internalBuffers.heapBuffer.length);

		//internalBuffers.heapBuffer.ptr[internalBuffers.heapBuffer.length] = '\0';
	}
	else
	{
		internalBuffers.stackBuffer.length = other.internalBuffers.stackBuffer.length;

		memset(internalBuffers.stackBuffer.ptr, 0, SSO_MAX_CHARS);
		memcpy(internalBuffers.stackBuffer.ptr, other.internalBuffers.stackBuffer.ptr, internalBuffers.stackBuffer.length);

		//internalBuffers.stackBuffer.ptr[internalBuffers.stackBuffer.length] = '\0';
	}
}

String::String(const char* Input, u32 length)
{
	if (!Input || *Input == '\0' || length <= 0)
		return;

	char* direct = GetRecommendedBufferBySize(length);
	memcpy(direct, Input, length);
}

String::String(const StringView& str)
{
	if (!str.ptr || *str.ptr == '\0' || str.size <= 0)
		return;

	char* direct = GetRecommendedBufferBySize(str.size);
	memcpy(direct, (char*)str.ptr, str.size);
	//direct[str.size] = '\0';
}

String String::operator+(const String& Other)
{
	return *this;
}

String& String::operator+=(const char* other)
{
	if (!other)
		return *this;

	String tmp(other);
	return (*this += tmp);
}

String String::Format(const char* format, ...)
{
	va_list a;
	va_start(a, format);

	const int sizeForVA = vsnprintf(nullptr, 0, format, a);

	char fixedFormattingBuffer[256 + 1] = { '\0' };
	char* formattedBuffer = sizeForVA <= 256 ? fixedFormattingBuffer : GetMemoryManager()->Allocate<char>(sizeForVA + 1);

	const int result = vsnprintf(formattedBuffer, sizeForVA + 1, format, a);
	va_end(a);

	String stringized(formattedBuffer);

	// if (sizeForVA > 256 ) GetMemoryManager()->Deallocate(formattedBuffer);
	return stringized;
}

constexpr char* String::GetRecommendedBufferBySize(u32 size)
{
	if (size > SSO_MAX_CHARS)
	{
		internalBuffers.heapBuffer.capacity = (u32)(size * 1.5f);
		internalBuffers.heapBuffer.capacity |= 0x80000000;
		internalBuffers.heapBuffer.length = size;

		internalBuffers.heapBuffer.ptr = GetMemoryManager()->Allocate<char>(internalBuffers.heapBuffer.capacity & 0x7FFFFFFF);
		return internalBuffers.heapBuffer.ptr;
	}
	else
	{
		internalBuffers.stackBuffer.length = size;
		return internalBuffers.stackBuffer.ptr;
	}

	return nullptr;
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		if (!other.SitsOnStack())
		{
			internalBuffers.heapBuffer.capacity = other.internalBuffers.heapBuffer.capacity;
			internalBuffers.heapBuffer.length = other.internalBuffers.heapBuffer.length;

			u32 realCap = internalBuffers.heapBuffer.capacity & 0x7FFFFFFF;
			internalBuffers.heapBuffer.ptr = GetMemoryManager()->Allocate<char>(realCap);

			memset(internalBuffers.heapBuffer.ptr, 0, realCap);
			memcpy(internalBuffers.heapBuffer.ptr, other.internalBuffers.heapBuffer.ptr, internalBuffers.heapBuffer.length);
			internalBuffers.heapBuffer.ptr[internalBuffers.heapBuffer.length] = '\0';
		}
		else
		{
			internalBuffers.stackBuffer.length = other.internalBuffers.stackBuffer.length;

			memset(internalBuffers.stackBuffer.ptr, 0, SSO_MAX_CHARS);
			memcpy(internalBuffers.stackBuffer.ptr, other.internalBuffers.stackBuffer.ptr, internalBuffers.stackBuffer.length);
			internalBuffers.stackBuffer.ptr[internalBuffers.stackBuffer.length] = '\0';
		}
	}

	return *this;
}

String& String::operator=(String&& other) noexcept
{
	if (this != &other)
	{
		if (!other.SitsOnStack())
		{
			internalBuffers.heapBuffer.capacity = other.internalBuffers.heapBuffer.capacity;
			internalBuffers.heapBuffer.length = other.internalBuffers.heapBuffer.length;
			internalBuffers.heapBuffer.ptr = other.internalBuffers.heapBuffer.ptr;
			
			other.internalBuffers.heapBuffer.capacity = 0;
			other.internalBuffers.heapBuffer.length = 0;
			other.internalBuffers.heapBuffer.ptr = nullptr;
		}
		else
		{
			internalBuffers.stackBuffer.length = other.internalBuffers.stackBuffer.length;
			memmove(internalBuffers.stackBuffer.ptr, other.internalBuffers.stackBuffer.ptr, internalBuffers.stackBuffer.length);

			memset(other.internalBuffers.stackBuffer.ptr, 0, SSO_MAX_CHARS);
			other.internalBuffers.stackBuffer.length = 0;
		}
	}

	return *this;
}

constexpr String& String::operator=(const char* other)
{
	const u32 otherSize = Count(other);

	char* target = GetRecommendedBufferBySize(otherSize);
	StringCopy(other, target);

	return *this;
}

bool String::operator==(const String& Other) const
{
	return strcmp(!SitsOnStack() ? internalBuffers.heapBuffer.ptr : internalBuffers.stackBuffer.ptr, !Other.SitsOnStack() ? Other.internalBuffers.heapBuffer.ptr : Other.internalBuffers.stackBuffer.ptr) == 0;
}

String& String::operator=(const StringView& other)
{
	if (strcmp(Chr(), (char*)other.ptr) == 0)
		return *this;

	char* target = GetRecommendedBufferBySize(other.size);
	memcpy(target, (char*)other.ptr, other.size);
	target[other.size] = '\0';

	return *this;
}

String& String::operator+=(const String& other)
{
	MR_ASSERT(false, "Broken function do not use!");

	return *this;
}