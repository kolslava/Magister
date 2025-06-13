set(SHARED_LIB SharedApplib)

project(${SHARED_LIB} CXX)

file(GLOB_RECURSE SHARED_LIB_SOURCES "src/*.cpp")

add_library(${SHARED_LIB} STATIC ${SHARED_LIB_SOURCES})

target_include_directories(${SHARED_LIB} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Лінкуємо spdlog та nlohmann_json
# PUBLIC, щоб заголовки цих бібліотек були доступні тим, хто використовує ${SHARED_LIB}
target_link_libraries(${SHARED_LIB} PUBLIC spdlog::spdlog nlohmann_json::nlohmann_json)