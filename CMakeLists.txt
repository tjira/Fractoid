cmake_minimum_required(VERSION 3.16)
project(Fractoid)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/gui)

find_package(OpenMP QUIET)
if(OpenMP_CXX_FOUND)
	set(CMAKE_CXX_FLAGS -fopenmp)
endif()

add_library(fractoid
	lib/fractoid/src/algorithm.cpp
	lib/fractoid/src/burningship.cpp
	lib/fractoid/src/complex.cpp
	lib/fractoid/src/image.cpp
	lib/fractoid/src/julia.cpp
	lib/lodepng/lodepng.cpp
	lib/fractoid/src/mandelbrot.cpp
	lib/fractoid/src/manowar.cpp
	lib/fractoid/src/phoenix.cpp
)

add_executable(fractoid-cli src/cli.cpp)
target_link_libraries(fractoid-cli fractoid)

find_package(Qt5Widgets QUIET)
if(Qt5Widgets_FOUND)
	add_executable(
		fractoid-gui
		src/gui.cpp
		inc/window.h
		src/window.cpp
	)
	target_link_libraries(fractoid-gui fractoid Qt5::Widgets)
	set_target_properties(fractoid-gui PROPERTIES AUTOMOC ON AUTOUIC ON)
endif()
