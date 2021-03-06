#[[
    Steps to build a CMake project:
        1. Make a minimum CMakeList.txt project file.
        2. Open a command line and move to the directory where you made the CMakeList.txt file.
        3. Make a sub-directory like 'build' not to make CMake-related files in the current directory.
        4. Move to the sub-directory where you made at the step-3.
        5. Run the cmake command:
            ex.> cmake ..
                    ; This will create CMake-related files in the current directory by using the ../CMakeList.txt.
                 cmake .. -DCMAKE_BUILD_TYPE=Debug
                    ; You can define some CMake variables by using the '-D' option.
        6. Run the 'make' command.
        7. If the project is for an executable, then you can find your executable in the current directory.
        8. You can use a command like following to remove all the CMake-related files in the current directory:
            ex.> rm -rf ./*
                    ; NOTE: Use this with caution.
]]

cmake_minimum_required(VERSION 3.0.0)
project(mfsm_test VERSION 0.0.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(BOOST_ROOT "../../boost_1_63_0")
find_package(Boost 1.63.0)
if(Boost_FOUND)
    include_directories(${BOOST_ROOT})
else()
    message(FATAL_ERROR "Could not find boost!")
endif()

set(SKULL_ROOT "../../skull/include")
include_directories(${SKULL_ROOT})

set(MFSM_ROOT "../include")
include_directories(${MFSM_ROOT})

# build type
set(CMAKE_BUILD_TYPE "Debug")

# LLVM 5.0.0 base path
set(LLVM_5_0_0_BASE "/Users/gilhojang/clang/clang+llvm-5.0.0-x86_64-apple-darwin")

set(CMAKE_C_COMPILER  "${LLVM_5_0_0_BASE}/bin/clang")
set(CMAKE_CXX_COMPILER  "${LLVM_5_0_0_BASE}/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

# NOTE: This is required to make the linker, ld, find the LLVM 4.0.0 libraries.
link_directories(${LLVM_5_0_0_BASE}/lib)

set(SOURCE_FILES
        mfsm_etc_test.cpp
        mfsm_user_input_echo_test.cpp
        main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# NOTE: This should be placed AFTER the above 'add_executable' command.
target_link_libraries(${CMAKE_PROJECT_NAME} LINK_PUBLIC c++)
