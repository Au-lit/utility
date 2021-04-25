// � Copyright 2021 Ollivier Roberge
#ifndef RANDOM_VALUE_IMPL
#define RANDOM_VALUE_IMPL

#include <limits>
#include <random>
#include <type_traits>

namespace Aulit {
    template<typename ResultType>
#if __has_cpp_attribute(nodiscard) >= 201603L
    [[nodiscard]]
#endif
    ResultType uniform_random_value(
        ResultType a = 0,
        ResultType b = std::numeric_limits<ResultType>::max()
    ) noexcept {
        thread_local std::random_device rd;
        thread_local std::default_random_engine random_engine(rd());
        if constexpr (std::is_floating_point<ResultType>::value)
            return std::uniform_real_distribution<ResultType>(a, b).operator()(random_engine);
        else if constexpr (std::is_same<ResultType, bool>::value)
            return std::bernoulli_distribution().operator()(random_engine);
        else return std::uniform_int_distribution<ResultType>(a, b).operator()(random_engine);
    }
}

#endif // !defined(RANDOM_VALUE_IMPL)