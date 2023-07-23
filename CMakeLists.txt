cmake_minimum_required(VERSION 3.0.0)
project(TestCompressing VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(TestCompressing main_lzw.cpp)

#add_subdirectory(miniz)
#target_link_libraries(TestCompressing miniz)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
