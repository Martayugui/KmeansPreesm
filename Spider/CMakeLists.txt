cmake_minimum_required(VERSION 2.6)

# *******************************************
# ************* CMake Content ***************
# *******************************************
# This CMake create a workspace containing the following projects
# 
# Programs
#  - sobel

project(Sobel)
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)



# Add definition for relative path into project
add_definitions( -DPROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

# Set to 1 to activate debug compilation (0 for release)
set(DEBUG 0)

if(${DEBUG})
		MESSAGE("Generate Debug project")
		set(CMAKE_BUILD_TYPE Debug)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -pg -Wall")
else()
		MESSAGE("Generate Release project")
		set(CMAKE_BUILD_TYPE Release)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")
endif()
#add libmath during non visual studio builds
set(CMAKE_EXTRA_LIB lm)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake_modules/")

# *******************************************
# ************** SDL2 LIBRARY ***************
# *******************************************
Find_Package (SDL2 REQUIRED )

if(NOT SDL2_FOUND)
  MESSAGE(FATAL_ERROR "SDL not found !")
endif()

# ******************************************
# ************ SDL2TTF LIBRARY *************
# ******************************************
Find_Package (SDL2TTF REQUIRED )

if(NOT SDL2TTF_FOUND)
  MESSAGE(FATAL_ERROR "SDL2TTF not found !")
endif()

# ***********************
# ******** SPIDER *******
# ***********************
# find the spider folder in the libspider directory.
file(GLOB SPIDERDIR "${LIBS_DIR}/spider")
set(ENV{SPIDERDIR} ${SPIDERDIR})
 
Find_Package (Spider REQUIRED )
if(NOT SPIDER_FOUND)
  MESSAGE(FATAL_ERROR "SPIDER not found !")
endif()

# *******************************************
# **************** Sobel.exe  ***************
# *******************************************

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${THREADS_PTHREADS_INCLUDE_DIR} ${SPIDER_INCLUDE_DIR})

file(
	GLOB_RECURSE
	source_files
    ./generated/*.cpp
	./src/*.cpp
)

file(
    GLOB_RECURSE
    header_files
    ./src/*.h
    ./generated/*.h
    ./include/*.h
)
add_executable( sobel ${source_files} ${header_files})
target_link_libraries(sobel ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${SPIDER_LIBRARY})
target_link_libraries(sobel m)
set(CMAKE_C_FLAGS "-std=gnu99 -lm")
