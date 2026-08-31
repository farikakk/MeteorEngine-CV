/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Core.essential.h>

namespace HAL
{
	namespace Paths
	{
		CORE_API String GetExecutablePath();
	
		CORE_API String GetEffectsPath();

		CORE_API void NormalizePath(String& inPath, const char to = '/', const char from = '\\');

		CORE_API StringView GetExtension(const String& inPath);
	};
}