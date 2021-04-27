// © Copyright 2021 Ollivier Roberge
#ifndef RANDOM_VALUE_IMPL
#define RANDOM_VALUE_IMPL

#include <limits>
#include <random>
#include <type_traits>
#include <cstddef>

namespace Aulit {
	// Yes, this enum is not denoted with eTheName; Sue me for that if you want.
	enum reseed_engine : unsigned int {};

	/// \breif Returns a random value in the interval [a, b].
	/// <typeparam name="ResultType">
	/// If this is Aulit::reseed_engine :
	/// 1. The function will reseed the internal random engine.
	/// 2. The function will return the seed used.
	/// </typeparam>
	/// \param a The lowest value possibly returned.
	/// \param b The highest value possibly returned.
	/// \return A random value in the interval [a, b].
	template<typename ResultType>
	// VV when reseeding it's way too annoying...
//#if __has_cpp_attribute(nodiscard) >= 201603L
	//[[nodiscard]]
//#endif
	ResultType uniform_random_value(
		ResultType a = static_cast<ResultType>(0),
		ResultType b = static_cast<ResultType>(std::numeric_limits<ResultType>::max())
	) noexcept {
		thread_local std::random_device rd;
		thread_local std::default_random_engine random_engine(rd());
		if constexpr (std::is_same<ResultType, reseed_engine>::value) {
			const auto seed = rd();
			random_engine.seed(seed);
			return static_cast<ResultType>(seed);
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