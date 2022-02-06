cmake_minimum_required(VERSION 3.2)
project(SimpleDrawingFramework)


### Setup C++ Project ###
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/w[0-4]")
        string(REGEX REPLACE "/w[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()


### Create Target "SimpleDrawingFramework" ###
set(src
    source/test.cpp
)

add_executable(SimpleDrawingFramework ${src})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT SimpleDrawingFramework)


### Setup thirdparty modules ###
# SDL2 #
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/include)
if(WIN32)
    find_library(SDL2_LIB SDL2 ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2MAIN_LIB SDL2main ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    target_link_libraries(SimpleDrawingFramework ${SDL2_LIB})
    target_link_libraries(SimpleDrawingFramework ${SDL2MAIN_LIB})
    target_link_libraries(SimpleDrawingFramework opengl32)
    target_link_libraries(SimpleDrawingFramework version)
    target_link_libraries(SimpleDrawingFramework winmm)
    target_link_libraries(SimpleDrawingFramework imm32)
    target_link_libraries(SimpleDrawingFramework setupapi)
endif()
