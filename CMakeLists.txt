cmake_minimum_required(VERSION 3.16)
project(Island VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# 编译选项
# -------------------------
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -------------------------
# SDL3 系列库
# -------------------------
option(SDL_STATIC "Build SDL3 as static library" OFF)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_mixer EXCLUDE_FROM_ALL)

# -------------------------
# FreeType 子模块
# -------------------------
add_subdirectory(external/freetype EXCLUDE_FROM_ALL)
# 给 SDL_ttf 需要的 target 创建别名
add_library(Freetype::Freetype ALIAS freetype)
set(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/freetype/include")
set(FREETYPE_LIBRARY Freetype::Freetype)

# -------------------------
# SDL_ttf（依赖 FreeType）
# -------------------------
# 告诉 SDL_ttf 使用我们编译的 FreeType
set(SDL_TTF_USE_INTERNAL_FREETYPE OFF CACHE BOOL "" FORCE)
set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}" CACHE PATH "" FORCE)
set(FREETYPE_LIBRARY "${FREETYPE_LIBRARY}" CACHE FILEPATH "" FORCE)
add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)

# -------------------------
# Header-only 库
# -------------------------
add_subdirectory(external/glm EXCLUDE_FROM_ALL)
add_subdirectory(external/json EXCLUDE_FROM_ALL)
add_subdirectory(external/spdlog EXCLUDE_FROM_ALL)

# -------------------------
# 游戏源代码
# -------------------------


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/main.cpp
    src/engine/core/*.hpp
    src/engine/core/*.cpp
    src/engine/resource/*.hpp
    src/engine/resource/*.cpp
    src/engine/render/*.hpp
    src/engine/render/*.cpp
    src/engine/input/*.hpp
    src/engine/input/*.cpp
)


add_executable(Island ${SOURCES})
target_include_directories(Island PRIVATE ${CMAKE_SOURCE_DIR}/src)

# -------------------------
# 链接库
# -------------------------
target_link_libraries(Island PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
    Freetype::Freetype
    glm::glm
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

# -------------------------
# 运行时资源路径
# -------------------------
target_compile_definitions(Island PRIVATE
    ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# -------------------------
# Windows 下 DLL 拷贝
# -------------------------
if(WIN32 AND NOT SDL_STATIC)
    set(SDL_DLLS
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
        $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
    )

    foreach(DLL ${SDL_DLLS})
        add_custom_command(TARGET Island POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}"
            $<TARGET_FILE_DIR:Island>
        )
    endforeach()
endif()
