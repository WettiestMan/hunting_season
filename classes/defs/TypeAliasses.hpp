#ifndef TYPE_ALIASSES_HPP_HNSS
#define TYPE_ALIASSES_HPP_HNSS

#include <cstdint>
#include <chrono>

// redefining some types (it seems they look like Rust type names now)
using i8 = std::int8_t ;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// haven't defined isize and usize because of some confusion
// I mean, if we check the internet isize is intptr_t and usize
// is uintptr_t. However, since usize is also used for indexing,
// should I alias it to size_t? Because, while you can use 
// uintptr_t to index an array, it defines a different intention
// than size_t, and aliasing it to size_t would fit my needs better
// since I don't think I'll be doing reinterpret_cast from pointer
// to integer in this project. But if I do that, I now have an
// inconsistency between my typedefs, since isize is intptr_t but
// usize is not uintptr_t but size_t. Should I change my type alias
// for isize as well to something like ptrdiff_t or ssize_t? (ssize_t
// may not even work as a signed version of size_t since ssize_t may
// be implemented to only accept -1 for negative numbers)

// typedefs for long C++ standard library types (decided to respect their
// original naming conventions)

using stopwatch = std::chrono::steady_clock;
using instant = std::chrono::time_point<stopwatch>;
using micros = std::chrono::microseconds;
using millis = std::chrono::milliseconds;
using secs = std::chrono::seconds;

// I decided not to alias std::string_view since it's not that bad to type
// (unlike std::chrono::time_point<std::chrono::steady_clock>)

#endif