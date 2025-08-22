cmake_minimum_required(VERSION 3.16)
project(Island LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# ---------------- SDL3 ----------------
# 设置 SDL3 静态/动态库
option(SDL_STATIC "Build SDL3 as static library" OFF)  # OFF 默认动态库
add_subdirectory(external/SDL)

# ---------------- GLM ----------------
add_subdirectory(external/glm)

# ---------------- nlohmann-json ----------------
add_subdirectory(external/json)

# ---------------- spdlog ----------------
add_subdirectory(external/spdlog)

# ---------------- 游戏源代码 ----------------
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(Island ${GAME_SOURCES})

target_link_libraries(Island PRIVATE
    SDL3::SDL3
    glm::glm
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

target_compile_definitions(Island PRIVATE
    ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# ---------------- Windows 下 DLL 拷贝 ----------------
if (WIN32 AND NOT SDL_STATIC)
    add_custom_command(TARGET Island POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/external/SDL/SDL3.dll"  # 源 DLL
        $<TARGET_FILE_DIR:Island>                     # 拷贝到可执行文件目录
    )
endif()
