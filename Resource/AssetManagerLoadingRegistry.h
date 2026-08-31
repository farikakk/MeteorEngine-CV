/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <HAL/DataTypes.h>
#include <Special/Crypt.h>
#include <Core.essential.h>

class CORE_API AssetManagerLoadingRegistry
{
public:
	typedef struct IPayload* (*AssetLoadingTypeFunction)(const char*);
protected:
	struct Internal
	{
		u64 extension = 0;
		AssetLoadingTypeFunction extensionFunc = nullptr;
	};

public:
	static __declspec(noinline) void AddNew(const char* extensionName, AssetLoadingTypeFunction func) noexcept
	{
		const u64 value = Hash(extensionName);
		for (u8 i = 0; i < U8_MAX; i++)
		{
			if (functions[i].extension == 0)
			{
				functions[i] = { value, func };
				return;
			}
		}
	}

	static __declspec(noinline) void Call(const char* extensionName, const char* buffer) noexcept
	{
		const u64 value = Hash(++extensionName);
		for (u8 i = 0; i < U8_MAX; i++)
		{
			if (functions[i].extension == value)
				functions[i].extensionFunc(buffer);
		}
	}

private:
	static inline Internal functions[U8_MAX];
};

#define ASSETMANAGER_ADDNEW(extension, func) \
    static struct AutoRegister_##extension \
	{ \
        AutoRegister_##extension() \
		{ \
            AssetManagerLoadingRegistry::AddNew(#extension, func); \
        } \
    } GLOBAL_AUTOREG_##extension;