cmake_minimum_required(VERSION 3.2)
project(2DGameEngine)

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


### Setup thirdparty modules ###
# SDL2 #
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/include)
if(MSVC)
    link_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT 2DGameEngine)
endif()


### Create Target "2DGameEngine" ###
set(src
    source/test.cpp
)

add_executable(2DGameEngine ${src})
target_link_libraries(2DGameEngine SDL2)
target_link_libraries(2DGameEngine SDL2main)

if (MSVC)
    add_custom_command(
        TARGET 2DGameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64/SDL2.dll" $<TARGET_FILE_DIR:2DGameEngine>
    )
endif()
