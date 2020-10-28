/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/defer.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <defer.hpp/defer.hpp>

namespace
{
}

TEST_CASE("defer") {
    SECTION("simple") {
        int i = 0;
        {
            DEFER_HPP([&i]{ ++i; });
            REQUIRE(i == 0);
        }
        REQUIRE(i == 1);
    }

    SECTION("simple_with_arg") {
        int i = 0;
        {
            DEFER_HPP([](int& i){ ++i; }, std::ref(i));
            REQUIRE(i == 0);
        }
        REQUIRE(i == 1);
    }

    SECTION("simple_with_args") {
        int i = 0, j = 0;
        {
            DEFER_HPP([](int& i, int& j){ ++i; j += 2; }, std::ref(i), std::ref(j));
            REQUIRE(i == 0);
            REQUIRE(j == 0);
        }
        REQUIRE(i == 1);
        REQUIRE(j == 2);
    }

    SECTION("simple_with_exception") {
        int i = 0;
        try {
            DEFER_HPP([&i]{ ++i; });
            REQUIRE(i == 0);
            throw std::exception();
        } catch (...) {
        }
        REQUIRE(i == 1);
    }
}

TEST_CASE("error_defer") {
    SECTION("simple") {
        int i = 0;
        {
            ERROR_DEFER_HPP([&i]{ ++i; });
            REQUIRE(i == 0);
        }
        REQUIRE(i == 0);
    }

    SECTION("simple_with_arg") {
        int i = 0;
        {
            ERROR_DEFER_HPP([](int& i){ ++i; }, std::ref(i));
            REQUIRE(i == 0);
        }
        REQUIRE(i == 0);
    }

    SECTION("simple_with_args") {
        int i = 0, j = 0;
        {
            ERROR_DEFER_HPP([](int& i, int& j){ ++i; j += 2; }, std::ref(i), std::ref(j));
            REQUIRE(i == 0);
            REQUIRE(j == 0);
        }
        REQUIRE(i == 0);
        REQUIRE(j == 0);
    }

    SECTION("simple_with_exception") {
        int i = 0;
        try {
            ERROR_DEFER_HPP([&i]{ ++i; });
            REQUIRE(i == 0);
            throw std::exception();
        } catch (...) {
        }
        REQUIRE(i == 1);
    }
}

TEST_CASE("return_defer") {
    SECTION("simple") {
        int i = 0;
        {
            RETURN_DEFER_HPP([&i]{ ++i; });
            REQUIRE(i == 0);
        }
        REQUIRE(i == 1);
    }

    SECTION("simple_with_arg") {
        int i = 0;
        {
            RETURN_DEFER_HPP([](int& i){ ++i; }, std::ref(i));
            REQUIRE(i == 0);
        }
        REQUIRE(i == 1);
    }

    SECTION("simple_with_args") {
        int i = 0, j = 0;
        {
            RETURN_DEFER_HPP([](int& i, int& j){ ++i; j += 2; }, std::ref(i), std::ref(j));
            REQUIRE(i == 0);
            REQUIRE(j == 0);
        }
        REQUIRE(i == 1);
        REQUIRE(j == 2);
    }

    SECTION("simple_with_exception") {
        int i = 0;
        try {
            RETURN_DEFER_HPP([&i]{ ++i; });
            REQUIRE(i == 0);
            throw std::exception();
        } catch (...) {
        }
        REQUIRE(i == 0);
    }
}
