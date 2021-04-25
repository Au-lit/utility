#ifndef ENDIAN_HELPER_IMPL
#define ENDIAN_HELPER_IMPL

#include <bit>
#include <cstddef> // std::byte
#include <type_traits> // std::is_constant_evaluated

#if _MSC_VER && !__INTEL_COMPILER
#include <intrin.h>
#endif // _MSVC_VER && !__INTEL_COMPILER

namespace { // builtin aliases...
	[[nodiscard]] constexpr std::uint16_t bitSwap16(std::uint16_t value) noexcept {
		if (std::is_constant_evaluated())
			return std::rotl(value, 8);
		else {
#if __has_builtin(__builtin_bswap16)
			return __builtin_bswap16(value);
#elif _MSC_VER && !__INTEL_COMPILER
			return _byteswap_ushort(value);
#else
			return std::rotl(value, 8); // maybe check for _rotl8
#endif // _MSVC_VER
		}
	}

	[[nodiscard]] constexpr std::uint32_t bitSwap32(std::uint32_t value) noexcept {
		if (std::is_constant_evaluated())
			return ((((value) & 0xff000000) >> 24) |
				(((value) & 0x00ff0000) >> 8) |
				(((value) & 0x0000ff00) << 8) |
				(((value) & 0x000000ff) << 24));
		else {
#if __has_builtin(__builtin_bswap32)
			return __builtin_bswap32(value);
#elif _MSC_VER && !__INTEL_COMPILER
			return _byteswap_ulong(value);
#else
			return ((((value) & 0xff000000) >> 24) |
				(((value) & 0x00ff0000) >> 8) |
				(((value) & 0x0000ff00) << 8) |
				(((value) & 0x000000ff) << 24));
#endif
		}

	}

	[[nodiscard]] constexpr std::uint64_t bitSwap64(std::uint64_t value) noexcept {
		if (std::is_constant_evaluated())
			return
			((((value) & 0xff00000000000000ull) >> 56) |
				(((value) & 0x00ff000000000000ull) >> 40) |
				(((value) & 0x0000ff0000000000ull) >> 24) |
				(((value) & 0x000000ff00000000ull) >> 8) |
				(((value) & 0x00000000ff000000ull) << 8) |
				(((value) & 0x0000000000ff0000ull) << 24) |
				(((value) & 0x000000000000ff00ull) << 40) |
				(((value) & 0x00000000000000ffull) << 56));
		else {
#if __has_builtin(__builtin_bswap64)
			return __builtin_bswap64(value);
#elif _MSC_VER && !__INTEL_COMPILER
			return _byteswap_uint64(value);
#else
			return
				((((value) & 0xff00000000000000ull) >> 56) |
					(((value) & 0x00ff000000000000ull) >> 40) |
					(((value) & 0x0000ff0000000000ull) >> 24) |
					(((value) & 0x000000ff00000000ull) >> 8) |
					(((value) & 0x00000000ff000000ull) << 8) |
					(((value) & 0x0000000000ff0000ull) << 24) |
					(((value) & 0x000000000000ff00ull) << 40) |
					(((value) & 0x00000000000000ffull) << 56));
#endif
		}

	}
	// ??? Implement 128 bit
#if __has_builtin(__builtin_bswap128)
#endif
}
//namespace Aulit {
namespace endian {
	[[nodiscard]] constexpr auto swap_endian(const auto& data) noexcept {
		if constexpr (sizeof(data) == 1)
			return data;
		else if constexpr (sizeof(data) == 2)
			return bitSwap16(std::bit_cast<std::uint16_t>(data)); // avoid UB
		else if constexpr (sizeof(data) == 4)
			return bitSwap32(std::bit_cast<std::uint32_t>(data));
		else if constexpr (sizeof(data) == 8)
			return bitSwap64(std::bit_cast<std::uint64_t>(data));
		else {
			auto p = static_cast<std::byte*>(&data);
			for (std::size_t lo = 0, hi = sizeof(data) - 1;
				hi > lo;
				++lo, --hi) {
				std::swap(p[lo], p[hi]);
			}
			return std::bit_cast<decltype(data)>(p);
		}
	}

	[[nodiscard]] constexpr auto native_to_little(auto data) noexcept {
		if constexpr (std::endian::native == std::endian::big)
			return swap_endian(data);
		else return data;
	}

	[[nodiscard]] constexpr auto native_to_big(auto data) noexcept {
		if constexpr (std::endian::native == std::endian::little)
			return swap_endian(data);
		else return data;
	}

	[[nodiscard]] constexpr auto big_to_native(auto data) noexcept {
		if constexpr (std::endian::native == std::endian::little)
			return swap_endian(data);
		else return data;
	}

	[[nodiscard]] constexpr auto little_to_native(auto data) noexcept {
		if constexpr (std::endian::native == std::endian::big)
			return swap_endian(data);
		else return data;
	}
}
//}
#endif // !ENDIAN_HELPER_IMPL