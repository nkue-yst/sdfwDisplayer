cmake_minimum_required(VERSION 3.2)
project(2DGameEngine)


### Setup C++ Project ###
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/w[0-4]")
        string(REGEX REPLACE "/w[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()


### Create Target "2DGameEngine" ###
set(src
    source/test.cpp
)

add_executable(2DGameEngine ${src})


### Setup thirdparty modules ###
# SDL2 #
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/include)
if(WIN32)
    find_library(SDL2_LIB SDL2 ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2MAIN_LIB SDL2main ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    target_link_libraries(2DGameEngine ${SDL2_LIB})
    target_link_libraries(2DGameEngine ${SDL2MAIN_LIB})
    target_link_libraries(2DGameEngine opengl32)
    target_link_libraries(2DGameEngine version)
    target_link_libraries(2DGameEngine winmm)
    target_link_libraries(2DGameEngine imm32)
    target_link_libraries(2DGameEngine setupapi)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT 2DGameEngine)
endif()
