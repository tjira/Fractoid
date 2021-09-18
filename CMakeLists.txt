cmake_minimum_required(VERSION 3.16)
project(Fractoid)
set(CMAKE_CXX_STANDARD 17)

set(DEFAULT_BUILD_TYPE Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
	set(CMAKE_CXX_FLAGS -fopenmp)
endif()

add_library(fractoid
	inc/fractoid/src/algorithm.cpp
	inc/fractoid/src/burningship.cpp
	inc/fractoid/src/complex.cpp
	inc/fractoid/src/image.cpp
	inc/fractoid/src/julia.cpp
	inc/fractoid/lib/lodepng/lodepng.cpp
	inc/fractoid/src/mandelbrot.cpp
	inc/fractoid/src/manowar.cpp
	inc/fractoid/src/phoenix.cpp
)

add_executable(fractoid-cli src/cli.cpp)
target_link_libraries(fractoid-cli fractoid)
