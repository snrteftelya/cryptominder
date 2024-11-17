cmake_minimum_required(VERSION 3.26)
project(cryptominder)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

set(SOURCE
        src/Account.cpp
        headers/Account.h
        src/Wallet.cpp
        headers/Wallet.h
        src/main.cpp
        src/MenuAccountWallet.cpp
        headers/MenuAccountWallet.h
        src/DataBaseSchema.cpp
        headers/DataBaseSchema.h
        src/Transaction.cpp
        headers/Transaction.h
)

add_executable(cryptominder ${SOURCE})

include_directories("/opt/homebrew/Cellar/libpqxx/7.9.2/include")

target_link_libraries(cryptominder PRIVATE /opt/homebrew/Cellar/libpqxx/7.9.2/lib/libpqxx-7.9.dylib)

target_include_directories(cryptominder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/headers")
target_include_directories(cryptominder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")