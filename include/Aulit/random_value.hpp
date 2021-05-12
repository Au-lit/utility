// © Copyright 2021 Ollivier Roberge
// For the documentation see https://github.com/Au-lit/utility-functions/wiki/random_value.hpp
// For the license see https://github.com/Au-lit/utility-functions/blob/master/LICENSE
#ifndef RANDOM_VALUE_IMPL
#define RANDOM_VALUE_IMPL

#include <limits>
#include <random>
#include <type_traits>
#undef min
#undef max // weird that I have to do that
namespace Aulit {
	// Yes, this enum is not denoted with eTheName; Sue me for that if you want.
	enum reseed_engine : std::random_device::result_type {};

	/// \param a The lowest value possibly returned.
	/// \param b The highest value possibly returned.
	/// \return A random value in the interval [a, b].
	template<typename ResultType>
	ResultType uniform_random_value(
		ResultType a = std::numeric_limits<ResultType>::lowest(), // maybe ::min() is more appropriate...
		ResultType b = std::numeric_limits<ResultType>::max()
	) noexcept {
		thread_local std::random_device rd;
		thread_local std::default_random_engine random_engine(rd());
		if constexpr (std::is_same<ResultType, reseed_engine>::value) {
			const auto seed = rd();
			random_engine.seed(seed);
			return static_cast<ResultType>(seed); // no asm
		}
		else {
			if constexpr (std::is_floating_point<ResultType>::value)
				return std::uniform_real_distribution<ResultType>(a, b).operator()(random_engine);
			else if constexpr (std::is_same<ResultType, bool>::value)
				return std::bernoulli_distribution().operator()(random_engine);
			else return std::uniform_int_distribution<ResultType>(a, b).operator()(random_engine);
		}
	}
}

#endif // !defined(RANDOM_VALUE_IMPL)