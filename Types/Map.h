/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>
#include <HAL/DataTypes.h>
#include <Special/Crypt.h>

template <typename Key, typename Value>
class Map
{
	struct InternalContainerForMap
	{
		u64 hashKey;
		Value data;
	};

public:
	Map() = default;
	~Map()
	{
		container.Clear();
	}

	Map(const Map& other)
	{
		container = other.container;
	};

	Map(Map&& other)
	{
		container = (Map&&)other.container;
		other.container = nullptr;
	};

	Map& operator=(const Map& other)
	{
		if (this != &other)
		{
			container = other.container;
		}

		return *this;
	}

	Map& operator=(Map&& other) noexcept
	{
		if (this != &other)
		{
			container = std::move(other.container);
		}

		return *this;
	}

	Value& operator[](Key Input)
	{
		u64 id = Hash(Input);

		for (auto& ctr : container)
		{
			if (id == ctr.hashKey)
				return ctr.data;
		}

		container.Add({ id, Value()});
		return container[container.GetSize() - 1].data;
	};

	const Value& operator[](Key Input) const
	{
		u64 id = Hash(Input);
		for (const auto& ctr : container)
		{
			if (id == ctr.hashKey)
				return ctr.data;
		}

		static Value dummy;
		return dummy;
	};

	InternalContainerForMap* begin() { return container.Data(); }
	InternalContainerForMap* end() { return container.Data() + container.GetSize(); }
	const InternalContainerForMap* begin() const { return container.Data(); }
	const InternalContainerForMap* end() const { return container.Data() + container.GetSize(); }

	bool operator!=(const Map& other)
	{
		return container != other.container;
	}
protected:
	Array<InternalContainerForMap> container;
};

