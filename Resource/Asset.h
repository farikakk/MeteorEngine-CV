/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <HAL/DataTypes.h>

class IFile;

//* A list of the selectible common asset types
enum class AssetType : u8
{
	None = 0,
	Texture,
	Sound,
	Font,
	Script,
	Material,
	Model,
	Shader,

	Max
};

//* Base IPayload for all resources around the Engine
//* Inherit from IPayload to make your own container for your asset type.
struct IPayload
{
	virtual ~IPayload() noexcept = default;
};

class Asset
{
	friend class AssetManager;
public:
	Asset() = default;
	~Asset() noexcept = default;

	Asset(const Asset& old)
	{
		hashValAssetTypeFlags = old.hashValAssetTypeFlags;
	};

	Asset(Asset&& old) noexcept
	{
		hashValAssetTypeFlags = old.hashValAssetTypeFlags;
	};

	Asset& operator=(const Asset& old)
	{
		if (this != &old)
		{
			hashValAssetTypeFlags = old.hashValAssetTypeFlags;
		}

		return *this;
	};

	Asset& operator=(Asset&& old) noexcept
	{
		if (this != &old)
		{
			hashValAssetTypeFlags = old.hashValAssetTypeFlags;

			u64 hashVal = (hashValAssetTypeFlags >> 0) & 0xFFFFFFFFFFFFULL;

			u8 assetType = (u8)((hashValAssetTypeFlags >> 48) & 0xFFULL);
			u8 flags = (u8)((hashValAssetTypeFlags >> 56) & 0xFFULL);
		}

		return *this;
	};

protected:
	//* This is a combined value! 
	//* First will be the assetType second is the flags third the hash of the name in the manager
	u64 hashValAssetTypeFlags = 0;

	IPayload* dataField = nullptr;

	IFile* fileHandle = nullptr;
};

