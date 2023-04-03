# How to Contribute

## Setting up environment
To keep the code base clean and as bug-free as possible, `libmt` contributors should use clang-format, cmake-format, and linters. 

To install the necessary code formatters, please run the following:

```bash
pip install clang-format==14.0.6 cmake_format==0.6.11 pyyaml
```

Every commit against the `main` branch will be compiled, linted, and tested, so please make sure that all CI tests pass before you open a PR.

## How to format the code
After making sure that the dependencies are installed, you can use the following commands to format the code
```bash
cmake -S test -B build/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

## General guideline on adding C++ code
`libmt` welcomes any feature (that is not overengineered) from C++17 and beyond! This means more type safety, friendlier API that prevents user errors, and just cool language features that are here to stay. Therefore, in general, please avoid using old C++ features that are deemed outdated or bad style.

It's also nice to follow the C++ Core Guidelines ([link](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)). Please check out the doc when you are in doubt!

## Testing
For `libmt`, it's not sufficient to just add a cool feature. We want to make sure that each feature is as bug-free as possible. This necessitates thorough testing. `libmt` uses the [doctest](https://github.com/doctest/doctest) framework for unit testing. Any PR that is opened should make sure that the existing tests are not broken and that new tests should be added to make sure that the code coverage is at least 90%!

