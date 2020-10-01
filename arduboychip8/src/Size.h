#pragma once

// For size_t
#include <stddef.h>

template< typename Type, size_t size >
constexpr size_t getSize(const Type (&)[size]) noexcept
{
	return size;
}