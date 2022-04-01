cmake_minimum_required(VERSION 3.2)

### Setup Project ###
project(sdfwDisplayer CXX)


### Setup C++ ###
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
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


### Create Target "sdfwDisplayer" ###
file(GLOB TARGET_SRC source/*.cpp source/*.hpp)
set(SDFW_DISPLAYER_SRC ${TARGET_SRC})
add_executable(sdfwDisplayer ${SDFW_DISPLAYER_SRC})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT sdfwDisplayer)


### Setup thirdparty modules ###
# SDL2, SDL2_net #
if(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2/include)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2_net/include)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2_ttf/include)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2_gfx/include)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/include)
    find_library(SDL2_LIB SDL2 ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2MAIN_LIB SDL2main ${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64)
    find_library(SDL2_NET_LIB SDL2_net ${PROJECT_SOURCE_DIR}/thirdparty/SDL2_net/lib/win64)
    find_library(SDL2_TTF_LIB SDL2_ttf ${PROJECT_SOURCE_DIR}/thirdparty/SDL2_ttf/lib/win64)
    find_library(SDL2_GFX_LIB SDL2_gfx ${PROJECT_SOURCE_DIR}/thirdparty/SDL2_gfx/lib/win64)
    find_library(SDL2_MIX_LIB SDL2_mixer ${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64)
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/lib/win64/SDL2.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_net/lib/win64/SDL2_net.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_ttf/lib/win64/SDL2_ttf.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_gfx/lib/win64/SDL2_gfx.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/SDL2_mixer.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libmodplug-1.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libmpg123-0.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libogg-0.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libopus-0.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libopusfile-0.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libvorbis-0.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    add_custom_command(
        TARGET sdfwDisplayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/win64/libvorbisfile-3.dll" $<TARGET_FILE_DIR:sdfwDisplayer>
    )
    target_link_libraries(sdfwDisplayer ${SDL2_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2MAIN_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2_NET_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2_TTF_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2_GFX_LIB})
    target_link_libraries(sdfwDisplayer ${SDL2_MIX_LIB})
    target_link_libraries(sdfwDisplayer opengl32)
    target_link_libraries(sdfwDisplayer version)
    target_link_libraries(sdfwDisplayer winmm)
    target_link_libraries(sdfwDisplayer imm32)
    target_link_libraries(sdfwDisplayer setupapi)
elseif(APPLE)
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/thirdparty/sdl2-cmake-modules)

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(sdfwDisplayer SDL2::Main)

    find_package(SDL2_net REQUIRED)
    include_directories(${SDL2_net_INCLUDE_DIR})
    target_link_libraries(sdfwDisplayer SDL2::Net)
endif()


### Setup for font data ###
add_custom_command(
    TARGET sdfwDisplayer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/Font/NotoSansJP-Regular.otf" $<TARGET_FILE_DIR:sdfwDisplayer>
)
