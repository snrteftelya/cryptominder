cmake_minimum_required(VERSION 3.26)
project(cryptominder)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

set(SOURCE
        src/Account.cpp
        headers/Account.h
        src/Wallet.cpp
        headers/Wallet.h
        src/main.cpp
        src/MenuAccountWallet.cpp
        headers/MenuAccountWallet.h
)

add_executable(cryptominder ${SOURCE})

        target_include_directories(cryptominder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/headers")
target_include_directories(cryptominder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")