// © Copyright 2021 Ollivier Roberge
// For the documentation see https://github.com/Au-lit/utility-functions/wiki/is_char.hpp
// For the license see https://github.com/Au-lit/utility-functions/blob/master/LICENSE
#ifndef IS_CHAR_IMPL
#define IS_CHAR_IMPL

#include <type_traits>

namespace Aulit {
	template<typename T> struct is_char : std::false_type {};
	template<> struct is_char<char> : std::true_type {};
	template<> struct is_char<wchar_t> : std::true_type {};
	template<> struct is_char<char16_t> : std::true_type {};
	template<> struct is_char<char32_t> : std::true_type {};
	#if __cpp_char8_t >= 201811L
		template<> struct is_char<char8_t> : std::true_type {};
	#endif
	#if __cpp_inline_variables >= 201606L
		template<typename T> inline constexpr bool is_char_v = is_char<T>::value;
	#endif
	#if __cpp_concepts >= 201811L // for msvc to not break, otherwise 201907L
		template<typename T> concept character = is_char<T>::value;
	#endif 
}

#endif // !defined(IS_CHAR_IMPL)