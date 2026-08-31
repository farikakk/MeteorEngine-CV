/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once

#if defined(_WIN64) || defined(MR_SPECIAL_USEWIN)
#ifndef MR_PLATFORM_WINDOWS
#define MR_PLATFORM_WINDOWS 1
#endif // !MR_PLATFORM_WINDOWS


#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // Windows 10
#endif

#define	WIN32_LEAN_AND_MEAN
#define	NOMINMAX
#define NOSERVICE
#define NOMCX
#define NOHELP
#define NOKANJI

#include <Windows.h>

#endif // defined(_WIN64)