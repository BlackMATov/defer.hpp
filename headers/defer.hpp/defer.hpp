/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/defer.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
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
        class noncopyable {
        public:
            noncopyable(const noncopyable&) = delete;
            noncopyable& operator=(const noncopyable&) = delete;
        protected:
            noncopyable() = default;
            ~noncopyable() = default;
        };

        template < typename F, typename... Args >
        class defer_impl : noncopyable {
        public:
            template < typename UF >
            explicit defer_impl(
                UF&& f,
                std::tuple<Args...>&& args)
            : f_(std::forward<UF>(f))
            , args_(std::move(args)) {}

            virtual ~defer_impl() noexcept {
                if ( !dismissed_ ) {
                    std::apply(std::move(f_), std::move(args_));
                }
            }

            void dismiss() noexcept {
                dismissed_ = true;
            }
        private:
            F f_;
            std::tuple<Args...> args_;
            bool dismissed_{};
        };

        template < typename F, typename... Args >
        class error_defer_impl final : public defer_impl<F, Args...> {
        public:
            template < typename UF >
            explicit error_defer_impl(
                UF&& f,
                std::tuple<Args...>&& args)
            : defer_impl<F, Args...>(std::forward<UF>(f), std::move(args))
            , exceptions_(std::uncaught_exceptions()) {}

            ~error_defer_impl() noexcept final {
                if ( exceptions_ == std::uncaught_exceptions() ) {
                    this->dismiss();
                }
            }
        private:
            int exceptions_{};
        };

        template < typename F, typename... Args >
        class return_defer_impl final : public defer_impl<F, Args...> {
        public:
            template < typename UF >
            explicit return_defer_impl(
                UF&& f,
                std::tuple<Args...>&& args)
            : defer_impl<F, Args...>(std::forward<UF>(f), std::move(args))
            , exceptions_(std::uncaught_exceptions()) {}

            ~return_defer_impl() noexcept final {
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
        using defer_t = impl::defer_impl<
            std::decay_t<F>,
            std::decay_t<Args>...>;
        return defer_t(
            std::forward<F>(f),
            std::make_tuple(std::forward<Args>(args)...));
    }

    template < typename F, typename... Args >
    auto make_error_defer(F&& f, Args&&... args) {
        using defer_t = impl::error_defer_impl<
            std::decay_t<F>,
            std::decay_t<Args>...>;
        return defer_t(
            std::forward<F>(f),
            std::make_tuple(std::forward<Args>(args)...));
    }

    template < typename F, typename... Args >
    auto make_return_defer(F&& f, Args&&... args) {
        using defer_t = impl::return_defer_impl<
            std::decay_t<F>,
            std::decay_t<Args>...>;
        return defer_t(
            std::forward<F>(f),
            std::make_tuple(std::forward<Args>(args)...));
    }
}

#define DEFER_HPP_IMPL_PP_CAT(x, y) DEFER_HPP_IMPL_PP_CAT_I(x, y)
#define DEFER_HPP_IMPL_PP_CAT_I(x, y) x ## y

#ifdef __COUNTER__
    #define DEFER_HPP(...)\
        auto DEFER_HPP_IMPL_PP_CAT(generated_defer_, __COUNTER__) =\
            ::defer_hpp::make_defer(__VA_ARGS__)

    #define ERROR_DEFER_HPP(...)\
        auto DEFER_HPP_IMPL_PP_CAT(generated_error_defer_, __COUNTER__) =\
            ::defer_hpp::make_error_defer(__VA_ARGS__)

    #define RETURN_DEFER_HPP(...)\
        auto DEFER_HPP_IMPL_PP_CAT(generated_return_defer_, __COUNTER__) =\
            ::defer_hpp::make_return_defer(__VA_ARGS__)
#else
    #define DEFER_HPP(...)\
        auto DEFER_HPP_IMPL_PP_CAT(generated_defer_, __LINE__) =\
            ::defer_hpp::make_defer(__VA_ARGS__)

    #define ERROR_DEFER_HPP(...)\
        auto DEFER_HPP_IMPL_PP_CAT(generated_error_defer_, __LINE__) =\
            ::defer_hpp::make_error_defer(__VA_ARGS__)

    #define RETURN_DEFER_HPP(...)\
        auto DEFER_HPP_IMPL_PP_CAT(generated_return_defer_, __LINE__) =\
            ::defer_hpp::make_return_defer(__VA_ARGS__)
#endif
