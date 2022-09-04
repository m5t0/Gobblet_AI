# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)

project ("cpp")
project ("cpp_test")

# サブプロジェクトを含めます。
add_subdirectory ("cpp")
add_subdirectory ("cpp_test")
