project(opencv-mobile-test)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/../host-tools/gcc/riscv64-linux-musl-x86_64/bin/riscv64-unknown-linux-musl-gcc")
set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/../host-tools/gcc/riscv64-linux-musl-x86_64/bin/riscv64-unknown-linux-musl-g++")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d")

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv-mobile-4.11.0-milkv-duo/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

add_executable(opencv-mobile-test main.cpp)

target_link_libraries(opencv-mobile-test ${OpenCV_LIBS})
