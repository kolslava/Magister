set(SHARED_LIB SharedApplib)

add_library(${SHARED_LIB} STATIC
    src/Shared.cpp
    src/Logger.cpp
    src/JsonSerializer.cpp
    src/CryptoHelper.cpp
    src/SignatureManager.cpp
)

target_include_directories(${SHARED_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${SHARED_LIB} 
    PUBLIC spdlog::spdlog 
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC Threads::Threads
)