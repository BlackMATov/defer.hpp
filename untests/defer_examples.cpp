/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/defer.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <defer.hpp/defer.hpp>
#include "doctest/doctest.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

TEST_CASE("examples") {
    SUBCASE("basic_defer") {
        if ( FILE *file = std::fopen("output.txt", "a") ) {
            // defer will close the file after scope or on exception
            DEFER_HPP([file]{ std::fclose(file); });

            const char buffer[] = "hello world\n";
            if ( 12 != std::fwrite(buffer, sizeof(buffer[0]), std::strlen(buffer), file) ) {
                throw std::runtime_error("some exception");
            }
        }
    }

    SUBCASE("error_defer") {
        if ( FILE *file = std::fopen("output.txt", "a") ) {
            // defer will close the file after scope or on exception
            DEFER_HPP([file]{ std::fclose(file); });

            // error defer will be called on exception
            ERROR_DEFER_HPP([]{
                std::cerr << "there is something wrong" << std::endl;
            });

            const char buffer[] = "hello world\n";
            if ( 12 != std::fwrite(buffer, sizeof(buffer[0]), std::strlen(buffer), file) ) {
                throw std::runtime_error("some exception");
            }
        }
    }

    SUBCASE("return_defer") {
        if ( FILE *file = std::fopen("output.txt", "a") ) {
            // defer will close the file after scope or on exception
            DEFER_HPP([file]{ std::fclose(file); });

            // return defer will be called on successful scope exit
            RETURN_DEFER_HPP([]{
                std::cout << "all is ok!" << std::endl;
            });

            const char buffer[] = "hello world\n";
            if ( 12 != std::fwrite(buffer, sizeof(buffer[0]), std::strlen(buffer), file) ) {
                throw std::runtime_error("some exception");
            }
        }
    }
}
