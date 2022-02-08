cmake_minimum_required(VERSION 3.2)

### Setup Project ###
project(sdfwDisplayer)


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
set(SDFW_DISPLAYER_SRC
    source/Main.cpp
    source/MessageReceiver.hpp
    source/MessageReceiver.cpp
    source/sdfwDisplayer.hpp
    source/sdfwDisplayer.cpp
)

add_executable(sdfwDisplayer ${SDFW_DISPLAYER_SRC})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT sdfwDisplayer)


### Setup thirdparty modules ###
# SDL2, SDL2_net #
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2_net/include)
if(WIN32)
    find_library(SDL2_LIB SDL2 ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2MAIN_LIB SDL2main ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2_NET_LIB SDL2_net ${PROJECT_SOURCE_DIR}/thirdparty/SDL2_net/lib/win64)
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64/SDL2.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_net/lib/win64/SDL2_net.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    target_link_libraries(sdfwDisplayer ${SDL2_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2MAIN_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2_NET_LIB})
    target_link_libraries(sdfwDisplayer opengl32)
    target_link_libraries(sdfwDisplayer version)
    target_link_libraries(sdfwDisplayer winmm)
    target_link_libraries(sdfwDisplayer imm32)
    target_link_libraries(sdfwDisplayer setupapi)
endif()
