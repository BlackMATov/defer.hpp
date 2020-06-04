/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/defer.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <iostream>
#include <defer.hpp/defer.hpp>

TEST_CASE("examples") {
    SECTION("basic_defer") {
        if ( FILE *file = ::fopen("output.txt", "a") ) {
            // defer will close the file after scope or on exception
            DEFER([file]{ ::fclose(file); });

            const char buffer[] = "hello world\n";
            if ( 12 != ::fwrite(buffer, sizeof(buffer[0]), ::strlen(buffer), file) ) {
                throw std::runtime_error("some exception");
            }
        }
    }

    SECTION("error_defer") {
        if ( FILE *file = ::fopen("output.txt", "a") ) {
            // defer will close the file after scope or on exception
            DEFER([file]{ ::fclose(file); });

            // error defer will be called on exception
            ERROR_DEFER([]{
                std::cerr << "there is something wrong" << std::endl;
            });

            const char buffer[] = "hello world\n";
            if ( 12 != ::fwrite(buffer, sizeof(buffer[0]), ::strlen(buffer), file) ) {
                throw std::runtime_error("some exception");
            }
        }
    }

    SECTION("return_defer") {
        if ( FILE *file = ::fopen("output.txt", "a") ) {
            // defer will close the file after scope or on exception
            DEFER([file]{ ::fclose(file); });

            // return defer will be called on successful scope exit
            RETURN_DEFER([]{
                std::cout << "all is ok!" << std::endl;
            });

            const char buffer[] = "hello world\n";
            if ( 12 != ::fwrite(buffer, sizeof(buffer[0]), ::strlen(buffer), file) ) {
                throw std::runtime_error("some exception");
            }
        }
    }
}
