cmake_minimum_required(VERSION 3.2)

### Setup Project ###
project(sdfw_DrawingServer)


### Setup C++ ###
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


### Drawing server output directory ###
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


### Compiler settings ###
if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/w[0-4]")
        string(REGEX REPLACE "/w[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()


### Create Target "SDF_DrawingServer" ###
set(DRAWING_SERVER_SRC
    source/test.cpp
)

add_executable(sdfw_DrawingServer ${DRAWING_SERVER_SRC})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT sdfw_DrawingServer)


### Setup thirdparty modules ###
# SDL2 #
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/include)
if(WIN32)
    find_library(SDL2_LIB SDL2 ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2MAIN_LIB SDL2main ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    target_link_libraries(sdfw_DrawingServer ${SDL2_LIB})
    target_link_libraries(sdfw_DrawingServer ${SDL2MAIN_LIB})
    target_link_libraries(sdfw_DrawingServer opengl32)
    target_link_libraries(sdfw_DrawingServer version)
    target_link_libraries(sdfw_DrawingServer winmm)
    target_link_libraries(sdfw_DrawingServer imm32)
    target_link_libraries(sdfw_DrawingServer setupapi)
endif()
