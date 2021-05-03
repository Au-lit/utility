// © Copyright 2021 Ollivier Roberge
// For the license see https://github.com/Au-lit/utility-functions/blob/master/LICENSE
#ifndef SCOPE_EXIT_IMPL
#define SCOPE_EXIT_IMPL

#include <type_traits>
#include <functional>
#include <exception>
#include <utility>
//#include <any>

namespace Aulit {
	inline namespace standard {
		inline namespace experimental {
			template<typename Ef> class scope_exit {
			public:
				constexpr scope_exit() noexcept = default;
				template<typename Fn>
				constexpr scope_exit(Fn&& function) noexcept(std::is_nothrow_constructible_v<Fn>)
					: m_function(std::forward<Fn>(function)) {}
				constexpr scope_exit(scope_exit&& other) noexcept = default;
				scope_exit(const scope_exit& other) = delete;
				scope_exit& operator=(scope_exit&&) = default; // this is not standard but useful
				scope_exit& operator=(const scope_exit&) = delete;
				constexpr ~scope_exit() noexcept { if (m_canExit) std::invoke(m_function); }
				constexpr void release() noexcept { m_canExit = false; }
			private:
				Ef m_function;
				bool m_canExit = true;
			};

			template<typename Ef> class scope_fail {
			public:
				constexpr scope_fail() noexcept = default;
				template<typename Fn>
				constexpr scope_fail(Fn&& function) noexcept(std::is_nothrow_constructible_v<Fn>)
					: m_function(std::forward<Fn>(function)) {}
				constexpr scope_fail(scope_fail&& other) noexcept = default;
				scope_fail(const scope_fail& other) = delete;
				scope_fail& operator=(scope_fail&&) = default;
				scope_fail& operator=(const scope_fail&) = delete;
				constexpr ~scope_fail() noexcept { if (m_canExit && std::uncaught_exceptions() > 0) std::invoke(m_function); }
				constexpr void release() noexcept { m_canExit = false; }
			private:
				Ef m_function;
				bool m_canExit = true;
			};

			template<typename Ef> class scope_success {
			public:
				constexpr scope_success() noexcept = default;
				template<typename Fn>
				constexpr scope_success(Fn&& function) noexcept(std::is_nothrow_constructible_v<Fn>)
					: m_function(std::forward<Fn>(function)) {}
				constexpr scope_success(scope_success&& other) noexcept = default;
				scope_success(const scope_success& other) = delete;
				scope_success& operator=(scope_success&&) = default;
				scope_success& operator=(const scope_success&) = delete;
				constexpr ~scope_success() noexcept { if (m_canExit && std::uncaught_exceptions() < 1) std::invoke(m_function); }
				constexpr void release() noexcept { m_canExit = false; }
			private:
				Ef m_function;
				bool m_canExit = true;
			};

			template<typename Callable> [[nodiscard]] constexpr scope_exit<typename std::decay_t<Callable>> make_scope_exit(Callable&& F) {
				return scope_exit<typename std::decay_t<Callable>>(std::forward<Callable>(F));
			}

			template<typename Callable> [[nodiscard]] constexpr scope_fail<typename std::decay_t<Callable>> make_scope_fail(Callable&& F) {
				return scope_fail<typename std::decay_t<Callable>>(std::forward<Callable>(F));
			}

			template<typename Callable> [[nodiscard]] constexpr scope_success<typename std::decay_t<Callable>> make_scope_success(Callable&& F) {
				return scope_success<typename std::decay_t<Callable>>(std::forward<Callable>(F));
			}

			//template<class R, class D> class unique_resource {
			//	unique_resource();
			//	template <class RR, class DD> unique_resource(RR&& r, DD&& d) noexcept(/*see below*/);
			//	unique_resource(unique_resource&& other);
			//	~unique_resource();
			//	unique_resource& operator=(unique_resource&& other) noexcept(/*see below*/);
			//	void release() noexcept;
			//	void reset() noexcept { m_deleter(m_resource); }
			//	template <class RR> void reset(RR&& r);
			//	const R& get() const noexcept { return m_resource; }
			//	const D& get_deleter() const noexcept { return m_deleter; }
			//	std::add_lvalue_reference_t<std::remove_pointer_t<R>> operator*() const noexcept;
			//	R operator->() const noexcept;
			//private:
			//	R m_resource;
			//	D m_deleter;
			//};
		}
	}
}

#endif // !SCOPE_EXIT_IMPL