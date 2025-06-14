set(SERVER_APP ServerApp)

set(SERVER_APP_SOURCES 
    src/main.cpp
)

add_executable(${SERVER_APP} ${SERVER_APP_SOURCES})

target_include_directories(${SERVER_APP} PRIVATE
    ${CMAKE_SOURCE_DIR}/external/asio/asio/include
    ${CMAKE_SOURCE_DIR}/external/crow/include
#    ${CMAKE_CURRENT_SOURCE_DIR}/Shared-App/include
)

# Вказуємо, що agent_app використовує заголовки з ${SHARED_LIB}
target_link_libraries(${SERVER_APP} 
    PUBLIC SharedApplib
    PRIVATE sqlite_orm::sqlite_orm 
    PRIVATE SQLite::SQLite3
)