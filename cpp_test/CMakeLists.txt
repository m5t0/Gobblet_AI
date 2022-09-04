# CMakeList.txt : cpp の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします
# 単体テスト

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Now simply link against gtest or gtest_main as needed.
# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (cpp_test "search_test.cpp" )
include_directories(cpp_test "${PROJECT_SOURCE_DIR}/cpp")
target_link_libraries(cpp_test gtest_main cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpp_test PROPERTY CXX_STANDARD 20)
endif()

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++20")
    endif()
endif()

add_test(NAME my_test COMMAND $<TARGET_FILE:cpp_test>)
