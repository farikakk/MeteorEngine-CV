/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#include "AssetManager.h"
#include <Types/String.h>
#include <HAL/Paths.h>

#ifdef MR_PLATFORM_WINDOWS
#include <HAL/WindowsPlatform.h>
#endif // MR_PLATFORM_WINDOWS
#include "AssetManagerLoadingRegistry.h"

LOG_ADDCATEGORY(AssetManager);


AssetManager::AssetManager()
{
	assetsHeap = GetMemoryManager()->RequestNewRegion<MemoryBlockChained>("Asset Manager", 1_gB /* This is temporary! */);
};

bool AssetManager::LoadAsset(String& pathToAsset)
{
#ifdef MR_PLATFORM_WINDOWS
	HAL::Paths::NormalizePath(pathToAsset, '\\', '/');
	const StringView extensionType = HAL::Paths::GetExtension(pathToAsset);

	wchar_t* pathToAssetW = GetMemoryManager()->Allocate<wchar_t>(extensionType.size * sizeof(wchar_t));
	HAL::ConvertToWide(pathToAssetW, pathToAsset.Length(), pathToAsset.Chr());

	HANDLE temporaryHandle = CreateFileW(pathToAssetW, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_WRITE_THROUGH, nullptr);
	if (temporaryHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER lg = {};
		GetFileSizeEx(temporaryHandle, &lg);

		char* fileContentBuffer = GetMemoryManager()->Allocate<char>(lg.QuadPart);
		
		DWORD actuallyRead = 0;
		if (!ReadFile(temporaryHandle, fileContentBuffer, lg.QuadPart, &actuallyRead, nullptr))
		{
			MR_LOG(LogAssetManager, Error, "ReadFile returned: %d", ::GetLastError());
			
			GetMemoryManager()->Deallocate(fileContentBuffer);
			CloseHandle(temporaryHandle);
			return false;
		}

		AssetManagerLoadingRegistry::Call((char*)extensionType.ptr, fileContentBuffer);

		GetMemoryManager()->Deallocate(fileContentBuffer);
		CloseHandle(temporaryHandle);
	}
	else if (GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		MR_LOG(LogAssetManager, Error, "There is no such Asset named: %hs", pathToAsset.Chr());
	}
	else
	{
		MR_LOG(LogAssetManager, Error, "CreateFileW returned: %d At: %hs", ::GetLastError(), pathToAsset.Chr());
	}
#endif // MR_PLATFORM_WINDOWS

	return false;
}

static struct IPayload* MpFAS3(const char* asd)
{
	return nullptr;
}

ASSETMANAGER_ADDNEW(mp3, MpFAS3);
