/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <HAL/DataTypes.h>
#include <Types/StringUtils.h>
#include <Core.essential.h>

struct CORE_API StringView
{
	constexpr StringView() noexcept = default;

	constexpr StringView(const char* data, u32 length)
		: ptr((u8*)data)
		, size(length)
	{

	}

	constexpr StringView(const char* data)
		: ptr((u8*)data)
		, size(StringLength(data))
	{

	}

	StringView(const StringView&) = delete;
	
	StringView(StringView&& other) noexcept
	{
		ptr = other.ptr;
		size = other.size;

		other.ptr = nullptr;
		other.size = 0;
	}

	StringView& operator=(const StringView& other)
	{
		if (this != &other)
		{
			ptr = other.ptr;
			size = other.size;
		}
		
		return *this;
	}

	StringView& operator=(StringView&& other) noexcept
	{
		if (this != &other)
		{
			ptr = other.ptr;
			size = other.size;

			other.ptr = nullptr;
			other.size = 0;
		}
		
		return *this;
	}

	~StringView() = default;

	const u8* ptr = nullptr;

	u64 size = 0;

	constexpr const u8* begin() const { return ptr; };
	constexpr const u8* end() const { return ptr + size; };

	explicit operator bool() const
	{
		return ptr != nullptr && size > 0;
	}
};
