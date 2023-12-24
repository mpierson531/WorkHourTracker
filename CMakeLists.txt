cmake_minimum_required(VERSION 3.26)
project(WorkHourTracker)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

set(GCC_COVERAGE_COMPILE_FLAGS "-O0")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS})

add_subdirectory(gutils)
add_subdirectory(gmath)
add_subdirectory(gtiming)
add_subdirectory(gcolors)
add_subdirectory(gio)

add_subdirectory(tracker)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC gutils gcolors gio tracker)