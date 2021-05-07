// © Copyright 2021 Ollivier Roberge
// For the documentation see : https://github.com/Au-lit/utility-functions/wiki/endian.hpp
// For the license see : https://github.com/Au-lit/utility/blob/master/LICENSE

// Version History (might be moved to the wiki/docs if it gets too big):
// v2.0.0
//     - New system for compiler intrinsics and to disable them (#define AULIT_ENDIAN_NO_INTRINSICS)
//     - Fixed various bugs.
//     - Removed stupid GCC warnings.

#ifndef ENDIAN_HELPER_IMPL
#define ENDIAN_HELPER_IMPL

#include <bit>
#include <utility>
#include <cstddef> // std::byte
#include <type_traits> // std::is_constant_evaluated
#include <algorithm> // std::iter_swap

// We can't use a nice lib here for that...
constexpr auto endian_hpp_version = "v2.0.0";

namespace Aulit::endian {
	namespace {
		#pragma warning(push)
		#pragma warning(disable : 4067) // for __has_builtin to not break
		#pragma warning(disable : 4005)
		#pragma warning(disable : 4068) // GCC pragma
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wmacro-redefined"
		#pragma GCC diagnostic ignored "-Wno-unknown-pragmas"

		// ??? probably could be a constexpr
		#define AULIT_ENDIAN_INTRINSIC_MSVC false
		#define AULIT_ENDIAN_INTRINSIC_BSWAP false
		#define AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN false
		#define AULIT_ENDIAN_INTRINSIC_NEW_BUILTIN false

		#if _MSC_VER && !__INTEL_COMPILER
			#include <intrin.h>
			#define AULIT_ENDIAN_INTRINSIC_MSVC true
			#pragma intrinsic(_byteswap_uint64, _byteswap_ulong, _byteswap_ushort)
		#endif
		#if defined(__linux__) // probably won't ever be used but still could because of Nvidia HPC C++
			#include <byteswap.h>
			#define AULIT_ENDIAN_INTRINSIC_BSWAP true
		#endif
		#if defined(__clang__) && __has_builtin(__builtin_bswap32) && __has_builtin(__builtin_bswap64)
			#define AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN true
		#endif
		#if (defined(__GNUC__ ) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)))
			#define AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN true
		#endif
		#if defined(__clang__) && __has_builtin(__builtin_bswap16)
			#define AULIT_ENDIAN_INTRINSIC_NEW_BUILTIN true
		#endif
		#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
			#define AULIT_ENDIAN_INTRINSIC_NEW_BUILTIN true
		#endif
		#if defined(AULIT_ENDIAN_NO_INTRINSICS)
			#define AULIT_ENDIAN_INTRINSIC_MSVC false
			#define AULIT_ENDIAN_INTRINSIC_BSWAP false
			#define AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN false
			#define AULIT_ENDIAN_INTRINSIC_NEW_BUILTIN false
		#endif

		[[nodiscard]] constexpr std::uint16_t bitSwap16(std::uint16_t value) noexcept {
			if (std::is_constant_evaluated())
				return (value << 8) | (value >> 8);
			else {
			#if AULIT_ENDIAN_INTRINSIC_NEW_BUILTIN
				return __builtin_bswap16(value);
			#elif AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN
				return __builtin_bswap32(value << 16);
			#elif AULIT_ENDIAN_INTRINSIC_MSVC
				return _byteswap_ushort(value);
			#elif AULIT_ENDIAN_INTRINSIC_BSWAP
				return bswap_16(value);
			#else
				return (value << 8) | (value >> 8); // maybe check for _rotl16
			#endif
			}
		}

		[[nodiscard]] constexpr std::uint32_t bitSwap32(std::uint32_t value) noexcept {
			if (std::is_constant_evaluated())
				return ((((value) & 0xff000000) >> 24) |
					(((value) & 0x00ff0000) >> 8) |
					(((value) & 0x0000ff00) << 8) |
					(((value) & 0x000000ff) << 24));
			else {
			#if AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN
				return __builtin_bswap32(value);
			#elif AULIT_ENDIAN_INTRINSIC_MSVC
				return _byteswap_ulong(value);
			#elif AULIT_ENDIAN_INTRINSIC_BSWAP
				return bswap_32(value);
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
				return ((((value) & 0xff00000000000000ull) >> 56) |
					(((value) & 0x00ff000000000000ull) >> 40) |
					(((value) & 0x0000ff0000000000ull) >> 24) |
					(((value) & 0x000000ff00000000ull) >> 8) |
					(((value) & 0x00000000ff000000ull) << 8) |
					(((value) & 0x0000000000ff0000ull) << 24) |
					(((value) & 0x000000000000ff00ull) << 40) |
					(((value) & 0x00000000000000ffull) << 56));
			else {
			#if AULIT_ENDIAN_INTRINSIC_OLD_BUILTIN
				return __builtin_bswap64(value);
			#elif AULIT_ENDIAN_INTRINSIC_MSVC
				return _byteswap_uint64(value);
			#elif AULIT_ENDIAN_INTRINSIC_BSWAP
				return bswap_64(value);
			#else
				return ((((value) & 0xff00000000000000ull) >> 56) |
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
		// #if __has_builtin(__builtin_bswap128)

		#pragma GCC diagnostic pop // ignored "-Wmacro-redefined", ignored "-Wno-unknown-pragmas"
		#pragma warning(pop) // 4067, 4005, 4068
	}


	[[nodiscard]] constexpr auto swap_endian(auto data) noexcept {
		if constexpr (sizeof(data) == 1)
			return data;
		else if constexpr (sizeof(data) == 2)
			return std::bit_cast<decltype(data)>( // avoid UB
				bitSwap16(std::bit_cast<std::uint16_t>(data)));
		else if constexpr (sizeof(data) == 4)
			return std::bit_cast<decltype(data)>(
				bitSwap32(std::bit_cast<std::uint32_t>(data)));
		else if constexpr (sizeof(data) == 8)
			return std::bit_cast<decltype(data)>(
				bitSwap64(std::bit_cast<std::uint64_t>(data)));
		else {
			auto start = static_cast<std::byte*>(&data);
			auto dst = start + sizeof(data);
			for (dst--; dst > start; start++, dst--) std::iter_swap(dst, start);
			return data;
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

	constexpr void swap_endian_inplace(auto& data) noexcept {
		if constexpr (sizeof(data) == 2)
			data = std::bit_cast<decltype(data)>(bitSwap16(std::bit_cast<std::uint16_t>(data)));
		else if constexpr (sizeof(data) == 4)
			data = std::bit_cast<decltype(data)>(bitSwap32(std::bit_cast<std::uint32_t>(data)));
		else if constexpr (sizeof(data) == 8)
			data = std::bit_cast<decltype(data)>(bitSwap64(std::bit_cast<std::uint64_t>(data)));
		else {
			auto start = static_cast<std::byte*>(&data);
			auto dst = start + sizeof(data);
			for (dst--; dst > start; start++, dst--) std::iter_swap(dst, start);
		}
	}

	constexpr void native_to_little_inplace(auto& data) noexcept {
		if constexpr (std::endian::native == std::endian::big)
			swap_endian_inplace(data);
	}

	constexpr void native_to_big_inplace(auto& data) noexcept {
		if constexpr (std::endian::native == std::endian::little)
			swap_endian_inplace(data);
	}

	constexpr void big_to_native_inplace(auto& data) noexcept {
		if constexpr (std::endian::native == std::endian::little)
			swap_endian_inplace(data);
	}

	constexpr void little_to_native_inplace(auto& data) noexcept {
		if constexpr (std::endian::native == std::endian::big)
			swap_endian_inplace(data);
	}
}

#endif // !ENDIAN_HELPER_IMPL