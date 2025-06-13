cmake_minimum_required(VERSION 3.16)
project(DeviceProfiler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Встановлюємо шляхи для вихідних файлів
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Шукаємо системну бібліотеку SQLite3, вона потрібна для sqlite_orm
find_package(SQLite3 REQUIRED)

# Додаємо сабмодулі
add_subdirectory(external/json)
add_subdirectory(external/spdlog)
add_subdirectory(external/cpr)
add_subdirectory(external/sqlite_orm)

set(ASIO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include)
add_subdirectory(external/crow)

# Додаємо наші власні модулі
add_subdirectory(Shared-App)
add_subdirectory(Server-App)
add_subdirectory(Server-Agent-App)