cmake_minimum_required(VERSION 3.16)

project(ALBO VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(ARGON2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/argon2")

set(ARGON2_SOURCES
    "${ARGON2_ROOT}/src/argon2.c"
    "${ARGON2_ROOT}/src/core.c"
    "${ARGON2_ROOT}/src/blake2b.c"
    "${ARGON2_ROOT}/src/thread.c"
    "${ARGON2_ROOT}/src/encoding.c"
    "${ARGON2_ROOT}/src/ref.c"
)

set(OpenSSL_USE_STATIC_LIBS OFF)
set(OpenSSL_USE_MSVC_STATIC_RT OFF)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)

set(ALBO_INCLUDES
    "${PROJECT_SOURCE_DIR}/include"
    "${ARGON2_ROOT}/include"
    "${ARGON2_ROOT}/src"
    "${ARGON2_ROOT}/src/blake2"
)

add_library(albo_core
    src/sha256.cpp
    src/aes256.cpp
    src/X25519.cpp
    src/password.cpp
    src/Argon2ID_HashFunc.cpp
    ${ARGON2_SOURCES}
)

# Changed to PUBLIC
target_include_directories(albo_core PUBLIC ${ALBO_INCLUDES})
target_link_libraries(albo_core PRIVATE OpenSSL::Crypto)

add_executable(ALBO src/main.cpp)
# Added explicit include path
target_include_directories(ALBO PRIVATE ${ALBO_INCLUDES})
target_link_libraries(ALBO PRIVATE albo_core)

if (WIN32)
    add_executable(WINCLIENT src/network/win/client.cpp)
    add_executable(WINSERVER src/network/win/server.cpp)

    # Added explicit include paths for executables
    target_include_directories(WINCLIENT PRIVATE ${ALBO_INCLUDES})
    target_include_directories(WINSERVER PRIVATE ${ALBO_INCLUDES})

    target_link_libraries(WINCLIENT PRIVATE albo_core user32 kernel32 ws2_32)
    target_link_libraries(WINSERVER PRIVATE albo_core user32 kernel32 ws2_32)

    target_compile_definitions(ALBO PRIVATE ALBO_PLATFORM_WINDOWS)
    target_compile_definitions(WINCLIENT PRIVATE ALBO_PLATFORM_WINDOWS)
    target_compile_definitions(WINSERVER PRIVATE ALBO_PLATFORM_WINDOWS)

elseif (UNIX)
    add_executable(LINUXCLIENT src/network/linux/client.cpp)
    add_executable(LINUXSERVER src/network/linux/server.cpp)

    # Added explicit include paths for executables
    target_include_directories(LINUXCLIENT PRIVATE ${ALBO_INCLUDES})
    target_include_directories(LINUXSERVER PRIVATE ${ALBO_INCLUDES})

    target_link_libraries(LINUXCLIENT PRIVATE albo_core)
    target_link_libraries(LINUXSERVER PRIVATE albo_core)

    target_compile_definitions(ALBO PRIVATE ALBO_PLATFORM_UNIX)
endif()

set_target_properties(ALBO PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

if (WIN32)
    set_target_properties(WINCLIENT WINSERVER PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    )
elseif (UNIX)
    set_target_properties(LINUXCLIENT LINUXSERVER PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    )
endif()

add_custom_target(WelcomeMessage ALL
    COMMAND ${CMAKE_COMMAND} -E echo "A mind is born."
)