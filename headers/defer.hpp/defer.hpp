/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/defer.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2023, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <exception>
#include <tuple>
#include <type_traits>
#include <utility>

namespace defer_hpp
{
    namespace impl
    {
        template < typename F, typename... Args >
        class base_defer_t {
        public:
            base_defer_t() = delete;
            base_defer_t(base_defer_t&&) = delete;
            base_defer_t(const base_defer_t&) = delete;
            base_defer_t& operator=(base_defer_t&&) = delete;
            base_defer_t& operator=(const base_defer_t&) = delete;

            template < typename UF >
            explicit base_defer_t(UF&& f, std::tuple<Args...>&& args)
            : f_{std::forward<UF>(f)}
            , args_{std::move(args)} {}

            void dismiss() noexcept {
                dismissed_ = true;
            }

        protected:
            ~base_defer_t() noexcept {
                if ( !dismissed_ ) {
                    std::apply(std::move(f_), std::move(args_));
                }
            }

        private:
            F f_;
            std::tuple<Args...> args_;
            bool dismissed_{};
        };
    }

    namespace impl
    {
        template < typename F, typename... Args >
        class defer_t final : public base_defer_t<F, Args...> {
        public:
            defer_t() = delete;
            defer_t(defer_t&&) = delete;
            defer_t(const defer_t&) = delete;
            defer_t& operator=(defer_t&&) = delete;
            defer_t& operator=(const defer_t&) = delete;

            template < typename UF >
            explicit defer_t(UF&& f, std::tuple<Args...>&& args)
            : base_defer_t<F, Args...>{std::forward<UF>(f), std::move(args)} {}

            ~defer_t() noexcept = default;
        };

        template < typename F, typename... Args >
        class error_defer_t final : public base_defer_t<F, Args...> {
        public:
            error_defer_t() = delete;
            error_defer_t(error_defer_t&&) = delete;
            error_defer_t(const error_defer_t&) = delete;
            error_defer_t& operator=(error_defer_t&&) = delete;
            error_defer_t& operator=(const error_defer_t&) = delete;

            template < typename UF >
            explicit error_defer_t(UF&& f, std::tuple<Args...>&& args)
            : base_defer_t<F, Args...>{std::forward<UF>(f), std::move(args)}
            , exceptions_{std::uncaught_exceptions()} {}

            ~error_defer_t() noexcept {
                if ( exceptions_ == std::uncaught_exceptions() ) {
                    this->dismiss();
                }
            }

        private:
            int exceptions_{};
        };

        template < typename F, typename... Args >
        class return_defer_t final : public base_defer_t<F, Args...> {
        public:
            return_defer_t() = delete;
            return_defer_t(return_defer_t&&) = delete;
            return_defer_t(const return_defer_t&) = delete;
            return_defer_t& operator=(return_defer_t&&) = delete;
            return_defer_t& operator=(const return_defer_t&) = delete;

            template < typename UF >
            explicit return_defer_t(UF&& f, std::tuple<Args...>&& args)
            : base_defer_t<F, Args...>{std::forward<UF>(f), std::move(args)}
            , exceptions_{std::uncaught_exceptions()} {}

            ~return_defer_t() noexcept {
                if ( exceptions_ != std::uncaught_exceptions() ) {
                    this->dismiss();
                }
            }

        private:
            int exceptions_{};
        };
    }

    template < typename F, typename... Args >
    auto make_defer(F&& f, Args&&... args) {
        using defer_t = impl::defer_t<std::decay_t<F>, std::decay_t<Args>...>;
        return defer_t{std::forward<F>(f), std::make_tuple(std::forward<Args>(args)...)};
    }

    template < typename F, typename... Args >
    auto make_error_defer(F&& f, Args&&... args) {
        using defer_t = impl::error_defer_t<std::decay_t<F>, std::decay_t<Args>...>;
        return defer_t{std::forward<F>(f), std::make_tuple(std::forward<Args>(args)...)};
    }

    template < typename F, typename... Args >
    auto make_return_defer(F&& f, Args&&... args) {
        using defer_t = impl::return_defer_t<std::decay_t<F>, std::decay_t<Args>...>;
        return defer_t{std::forward<F>(f), std::make_tuple(std::forward<Args>(args)...)};
    }
}

#define DEFER_HPP_IMPL_PP_CAT(x, y) DEFER_HPP_IMPL_PP_CAT_I(x, y)
#define DEFER_HPP_IMPL_PP_CAT_I(x, y) x##y

#ifdef __COUNTER__
#    define DEFER_HPP(...) \
        auto DEFER_HPP_IMPL_PP_CAT(defer_hpp_generated_defer_, __COUNTER__) { ::defer_hpp::make_defer(__VA_ARGS__) }
#    define ERROR_DEFER_HPP(...) \
        auto DEFER_HPP_IMPL_PP_CAT(defer_hpp_generated_error_defer_, __COUNTER__) { ::defer_hpp::make_error_defer(__VA_ARGS__) }
#    define RETURN_DEFER_HPP(...) \
        auto DEFER_HPP_IMPL_PP_CAT(defer_hpp_generated_return_defer_, __COUNTER__) { ::defer_hpp::make_return_defer(__VA_ARGS__) }
#else
#    define DEFER_HPP(...) \
        auto DEFER_HPP_IMPL_PP_CAT(defer_hpp_generated_defer_, __LINE__) { ::defer_hpp::make_defer(__VA_ARGS__) }
#    define ERROR_DEFER_HPP(...) \
        auto DEFER_HPP_IMPL_PP_CAT(defer_hpp_generated_error_defer_, __LINE__) { ::defer_hpp::make_error_defer(__VA_ARGS__) }
#    define RETURN_DEFER_HPP(...) \
        auto DEFER_HPP_IMPL_PP_CAT(defer_hpp_generated_return_defer_, __LINE__) { ::defer_hpp::make_return_defer(__VA_ARGS__) }
#endif
