/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include "Asset.h"
#include <Types/Map.h>
#include <Core.essential.h>

class String;

class CORE_API AssetManager
{
	friend class Asset;
public:
	AssetManager();
	virtual ~AssetManager() noexcept = default;

	//* Basic asset load without decryption, should support any types of data file, but not reliable
	bool LoadAsset(String& pathToAsset);

protected:
	Map<const char*, Asset> assets;

	MemoryBlockChained* assetsHeap;
};

