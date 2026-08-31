/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include "DataTypes.h"
#include <Types/StringView.h>
#include <Core.essential.h>

//* Accessing mode: During execution, if you access the current from somewhere it behaves as the selected option.
enum Access : u8
{
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1
};

enum Share : u8
{
    Nothing = 0,
    Reading = 1 << 0,
    Writing = 1 << 1,
    Delete = 1 << 2
};

enum OpenAs : u8
{
    AlwaysCreate,
    CreateIfNotExist,
    JustOpen,
    OpenIfExist,
    Truncate
};

enum class Origin : u8
{
    Begin,
    Current,
    End
};

class CORE_API IFile
{
public:

	IFile() = default;
	virtual ~IFile() noexcept = default;

    //* Creates a file or opens if exist, define accessing mode with 'access', define sharing mode with 'share'
    bool Open(const StringView& name, const OpenAs openAs, const Access access, const Share share);

    void Read(char* returnedBuffer);

    u64 GetFileSize() const { return fileSize; };

protected:
	union
	{
		void* windowsAPIHandle = nullptr;

        u64 linuxHandle;
	};

    u64 fileSize = 0;
};