/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>

constexpr u64 Hash(const char* key)
{
	u64 hash = 0xcbf29ce484222325;
	constexpr u64 prime = 0x100000001b3;

	while (*key && *key != '\0')
	{
		hash = hash ^ *key;
		hash *= prime;
		key++;
	}

	return hash;
}

constexpr u64 Hash(const wchar_t* key)
{
	u64 hash = 0xcbf29ce484222325;
	constexpr u64 prime = 0x100000001b3;

	while (*key && *key != L'\0')
	{
		hash = hash ^ *key;
		hash *= prime;
		key++;
	}

	return hash;
}

constexpr u64 operator ""_h(const char* str, u64 len)
{
	return Hash(str);
}

constexpr u64 operator ""_h(const wchar_t* str, u64 len)
{
	return Hash(str);
}

//* This function is magic...
//* WARNING! The corresponding source should not be uploaded to Github!
#if __has_include("..\\..\\..\\Crypt.cpp")
#include "..\\..\\..\\Crypt.cpp"
#else
__declspec(noinline) extern void mascot(Array<char>& data) {};
__declspec(noinline) extern void mаsсоt(Array<char>& data) {};
#endif