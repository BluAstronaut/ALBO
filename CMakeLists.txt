cmake_minimum_required(VERSION 3.16)

project(ALBO
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)
find_package(SQLite3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(ARGON2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/argon2")

set(ARGON2_SOURCES
    "${ARGON2_ROOT}/src/argon2.c"
    "${ARGON2_ROOT}/src/core.c"
    "${ARGON2_ROOT}/src/blake2b.c"
    "${ARGON2_ROOT}/src/thread.c"
    "${ARGON2_ROOT}/src/encoding.c"
    "${ARGON2_ROOT}/src/ref.c"
)

set(ALBO_INCLUDES
    "${PROJECT_SOURCE_DIR}/include"
    "${ARGON2_ROOT}/include"
    "${ARGON2_ROOT}/src"
    "${ARGON2_ROOT}/src/blake2"
)

set(CRYPTO_SOURCES
    src/sha256.cpp
    src/aes256.cpp
    src/X25519.cpp
    src/network/universal/ed25519.cpp
)

set(NETWORK_SOURCES_UNIX
    src/network/universal/secure_socket.cpp
    src/network/universal/database.cpp
    src/network/universal/config_manager.cpp
    src/network/universal/professionalprovider.cpp
    src/network/linux/socket_manager.cpp
    src/network/linux/secure_mem.cpp
)

set(NETWORK_SOURCES_WIN
    src/network/universal/secure_socket.cpp
    src/network/universal/database.cpp
    src/network/universal/config_manager.cpp
    src/network/universal/professionalprovider.cpp
    src/network/win/network_boiler.cpp
    src/network/win/secure_mem.cpp
)

if(WIN32)
    set(ACTIVE_NETWORK_SOURCES ${NETWORK_SOURCES_WIN})
    set(PLATFORM_LIBS ws2_32 kernel32 user32)
else()
    set(ACTIVE_NETWORK_SOURCES ${NETWORK_SOURCES_UNIX})
    set(PLATFORM_LIBS "")
endif()

add_library(argon2 STATIC
    ${ARGON2_SOURCES}
    src/Argon2ID_HashFunc.cpp
    src/password.cpp
)
target_include_directories(argon2 PUBLIC ${ALBO_INCLUDES})

add_executable(ALBO
    ${CRYPTO_SOURCES}
    ${ACTIVE_NETWORK_SOURCES}
)
target_link_libraries(ALBO PRIVATE 
    OpenSSL::Crypto 
    OpenSSL::SSL 
    SQLite::SQLite3 
    argon2 
    ${PLATFORM_LIBS}
)
target_include_directories(ALBO PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(UNIX)
    # SERVER
    add_executable(LINUXSERVER
        src/network/linux/server.cpp
        src/network/server/auth_handler.cpp
        src/network/server/routing_handler.cpp
        src/network/server/rate_limiter.cpp
        ${CRYPTO_SOURCES}
        ${NETWORK_SOURCES_UNIX}
    )
    target_link_libraries(LINUXSERVER PRIVATE OpenSSL::Crypto OpenSSL::SSL SQLite::SQLite3 argon2)
    target_include_directories(LINUXSERVER PRIVATE ${PROJECT_SOURCE_DIR}/include)

    add_executable(LINUXCLIENT
        src/network/linux/client.cpp
        src/network/client/identity_manager.cpp
        src/network/client/crypto_service.cpp
        src/network/client/console_manager.cpp
        src/network/client/file_transfer_manager.cpp
        ${CRYPTO_SOURCES}
        ${NETWORK_SOURCES_UNIX}
    )
    target_link_libraries(LINUXCLIENT PRIVATE OpenSSL::Crypto OpenSSL::SSL SQLite::SQLite3 argon2)
    target_include_directories(LINUXCLIENT PRIVATE ${PROJECT_SOURCE_DIR}/include)

    add_executable(LINUX_CLIENT_GUI
        src/graphical/linux/main.cpp
        src/graphical/universal/main_window.cpp
        src/graphical/universal/network_controller.cpp
        src/graphical/universal/login_dialog.cpp
        src/network/client/identity_manager.cpp
        src/network/client/crypto_service.cpp
        src/network/client/file_transfer_manager.cpp
        ${CRYPTO_SOURCES}
        ${NETWORK_SOURCES_UNIX}
    )
    target_link_libraries(LINUX_CLIENT_GUI PRIVATE OpenSSL::Crypto OpenSSL::SSL SQLite::SQLite3 argon2 Qt6::Widgets)
    target_include_directories(LINUX_CLIENT_GUI PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/graphical)
    set_target_properties(LINUX_CLIENT_GUI PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
endif()

if(WIN32)
    add_executable(WINCLIENT
        src/network/win/client.cpp
        ${CRYPTO_SOURCES}
        ${NETWORK_SOURCES_WIN}
    )
    target_link_libraries(WINCLIENT PRIVATE OpenSSL::Crypto OpenSSL::SSL SQLite::SQLite3 argon2 ${PLATFORM_LIBS})
    target_include_directories(WINCLIENT PRIVATE ${PROJECT_SOURCE_DIR}/include)

    add_executable(WINSERVER
        src/network/win/server.cpp
        ${CRYPTO_SOURCES}
        ${NETWORK_SOURCES_WIN}
    )
    target_link_libraries(WINSERVER PRIVATE OpenSSL::Crypto OpenSSL::SSL SQLite::SQLite3 argon2 ${PLATFORM_LIBS})
    target_include_directories(WINSERVER PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif()

set(BIN_DIR ${PROJECT_BINARY_DIR}/bin)

set_target_properties(ALBO PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

if(TARGET LINUXSERVER)
    set_target_properties(LINUXSERVER PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif()
if(TARGET LINUXCLIENT)
    set_target_properties(LINUXCLIENT PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif()
if(TARGET LINUX_CLIENT_GUI)
    set_target_properties(LINUX_CLIENT_GUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif()
if(TARGET WINCLIENT)
    set_target_properties(WINCLIENT PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif()
if(TARGET WINSERVER)
    set_target_properties(WINSERVER PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif()