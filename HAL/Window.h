/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <Types/StringView.h>
#include <Types/String.h>
#include <Types/Vector.h>
#include <Core.essential.h>

class CORE_API Window
{
public:
	static Window* Create(const StringView& name, const u32 sx, const u32 sy, const u32 px, const u32 py);

	Window() noexcept = default;
	
	void Show();

	void Hide();

	void Rename(const StringView& newName);

	void Resize(const u32 x, const u32 y);

	void Destroy();

	void* GetNativeHandle() const { return windowsAPIHandle; };

	Vector2<u32> GetSize() const { return { x, y }; };
protected:

	Window(const StringView& name, const u32 x, const u32 y)
		: windowName(name)
	{
		this->x = x;
		this->y = y;
	}

	Window(const Window& old)
		: windowName(old.windowName)
		, x(old.x)
		, y(old.y)
	{

	}

	void* windowsAPIHandle = nullptr;

	String windowName;
	
	u32 x, y;
};

