# cmake_minimum_required(VERSION 3.16)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# project(interpreter)

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)

# find_package(LLVM REQUIRED CONFIG)

# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# include_directories(${LLVM_INCLUDE_DIRS})


# find_package(ZLIB REQUIRED)


# message("$(LLVM_INCLUDE_DIRS)" ${LLVM_INCLUDE_DIRS})

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# add_executable(interpreter Interpreter.cpp Parser.cpp Scanner.cpp)
# target_link_libraries(interpreter ${LLVM_TARGETS_TO_BUILD} ZLIB::ZLIB)


cmake_minimum_required(VERSION 3.4.3)
project(Kaleiscope)

# Set gdb
# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LLVM_INCLUDE_DIRS})
add_executable(interpreter Interpreter.cpp Parser.cpp Scanner.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti")

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader ScalarOpts
        ExecutionEngine OrcJIT native Analysis RuntimeDyld Object InstCombine mcjit)

# Link against LLVM libraries
target_link_libraries(interpreter ${llvm_libs})