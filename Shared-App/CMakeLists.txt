set(SHARED_LIB SharedApplib)

add_library(${SHARED_LIB} STATIC
    src/shared.cpp
)

#target_include_directories(${SHARED_LIB} PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    $<INSTALL_INTERFACE:include>
#)

target_include_directories(${SHARED_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${SHARED_LIB} 
    PUBLIC spdlog::spdlog 
    PUBLIC nlohmann_json::nlohmann_json
)