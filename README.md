# defer.hpp

> Go's defer implementation in C++17

[![linux][badge.linux]][linux]
[![darwin][badge.darwin]][darwin]
[![windows][badge.windows]][windows]
[![language][badge.language]][language]
[![license][badge.license]][license]

[badge.darwin]: https://img.shields.io/github/actions/workflow/status/BlackMATov/defer.hpp/.github/workflows/darwin.yml?label=Xcode&logo=xcode
[badge.linux]: https://img.shields.io/github/actions/workflow/status/BlackMATov/defer.hpp/.github/workflows/linux.yml?label=GCC%2FClang&logo=linux
[badge.windows]: https://img.shields.io/github/actions/workflow/status/BlackMATov/defer.hpp/.github/workflows/windows.yml?label=Visual%20Studio&logo=visual-studio
[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow
[badge.license]: https://img.shields.io/badge/license-MIT-blue

[darwin]: https://github.com/BlackMATov/defer.hpp/actions?query=workflow%3Adarwin
[linux]: https://github.com/BlackMATov/defer.hpp/actions?query=workflow%3Alinux
[windows]: https://github.com/BlackMATov/defer.hpp/actions?query=workflow%3Awindows
[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/MIT_License

[defer]: https://github.com/BlackMATov/defer.hpp

## Requirements

- [clang](https://clang.llvm.org/) **>= 7**
- [gcc](https://www.gnu.org/software/gcc/) **>= 7**
- [msvc](https://visualstudio.microsoft.com/) **>= 2019**
- [xcode](https://developer.apple.com/xcode/) **>= 10.3**

## Installation

[defer.hpp][defer] is a header-only library. All you need to do is copy the headers files from `headers` directory into your project and include them:

```cpp
#include "defer.hpp/defer.hpp"
```

Also, you can add the root repository directory to your [cmake](https://cmake.org) project:

```cmake
add_subdirectory(external/defer.hpp)
target_link_libraries(your_project_target PUBLIC defer.hpp::defer.hpp)
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
