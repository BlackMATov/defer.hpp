# defer.hpp

> Go's defer implementation in C++17

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]
[![paypal][badge.paypal]][paypal]

[badge.travis]: https://img.shields.io/travis/BlackMATov/defer.hpp/main.svg?logo=travis
[badge.appveyor]: https://img.shields.io/appveyor/ci/BlackMATov/defer-hpp/main.svg?logo=appveyor
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/defer.hpp/main.svg?logo=codecov
[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow.svg
[badge.license]: https://img.shields.io/badge/license-MIT-blue.svg
[badge.paypal]: https://img.shields.io/badge/donate-PayPal-orange.svg?logo=paypal&colorA=00457C

[travis]: https://travis-ci.org/BlackMATov/defer.hpp
[appveyor]: https://ci.appveyor.com/project/BlackMATov/defer-hpp
[codecov]: https://codecov.io/gh/BlackMATov/defer.hpp
[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/MIT_License
[paypal]: https://www.paypal.me/matov

[defer]: https://github.com/BlackMATov/defer.hpp

## Requirements

- [gcc](https://www.gnu.org/software/gcc/) **>= 7**
- [clang](https://clang.llvm.org/) **>= 5.0**
- [msvc](https://visualstudio.microsoft.com/) **>= 2017**

## Installation

[defer.hpp][defer] is a header-only library. All you need to do is copy the headers files from `headers` directory into your project and include them:

```cpp
#include "defer.hpp/defer.hpp"
```

Also, you can add the root repository directory to your [cmake](https://cmake.org) project:

```cmake
add_subdirectory(external/defer.hpp)
target_link_libraries(your_project_target defer.hpp)
```

## Examples

### Basic Defer

```cpp
if ( FILE *file = std::fopen("output.txt", "a") ) {
    // defer will close the file after scope or on exception
    DEFER_HPP([file]{ std::fclose(file); });

    const char buffer[] = "hello world\n";
    if ( 12 != std::fwrite(buffer, sizeof(buffer[0]), std::strlen(buffer), file) ) {
        throw std::runtime_error("some exception");
    }
}
```

### Error Defer

```cpp
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
```

### Return Defer

```cpp
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
```

## [License (MIT)](./LICENSE.md)
