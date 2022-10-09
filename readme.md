# Gobblet AI

`Gobblet AI` is codes written about depth-first search and analytics on transitions of positions of the board game `Gobblet`.

Constant variables are defined in `cpp/forward.h`, main functions are defined in `cpp/search.cpp`, and these things are used in `cpp_test/search_test.cpp`.
Codes which output csv files are also written in `cpp_test/search_test.cpp`.
Be care that some of the test codes in `cpp_test/search_test.cpp` are expected that the constant variables are those of 4x4 Gobblet.

Python codes used to investigate statistics and plot graphs using csv files are in the `python` folder and the plots and csv files are in the `output` folder.
Memos I wrote when developping are in `output/statistics.txt`.

# Development Environment
- Windows 11 Home 64bit
- Visual Studio 2022 Community version 17.3.3
- cmake tools for windows attached with Visual Studio 2022
- Python 3.9.13
- matplotlib 3.6.0

# How to build C++ source code
You have to connect to the internet to build for the first time because Google Test codes are downloaded from the internet.
I will explain how to build in Windows, but you will probably be able to build in Mac OS or Linux using g++ or clang++ like the following explanation `Use WSL`.
However, I can't guarantee that you can certainly build in Linux or Mac OS.

## Use Visual Studio
1. Please install Visual Studio. Make sure that features of Visual C++ and CMake are installed.
2. Please open the `Gobblet_AI` folder in Visual Studio.
3. You can build the project only to open the folder and build like usual Visual Studio Project because I used the CMake Project of Visual Studio.
4. Please make sure to run `cpp_test.exe`. If you try to run other files, it won't work well.

## Use WSL
Please build the codes of branch `wsl` because I used C++20 in codes of branch `master` but g++ and clang++ of WSL didn't support the features of C++20 I used when I checked if I can build them.
I stop using `std::format` and `std::bit_floor` in codes of branch `wsl`. 
Of course, g++ and clang++ of WSL may be updated and you may be able to build codes of branch `master`.

1. Please install (g++ or clang++) and cmake.
2. Please move to the `Gobblet_AI` folder like `cd ./Gobblet_AI`.
3. Please make a output folder like `mkdir ./out`.
4. Please move to the output folder like `cd ./out`.
5. Please use the command `cmake ..` .
6. Please use the command `cmake --build .` . If you want to build the codes as Release build, please use the command `cmake --build . --config Release`.
7. Please make sure to run `cpp_test.exe`. If you try to run other files, it won't work well.
