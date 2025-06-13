set(AGENT_APP ServerAgentApp)

project(${AGENT_APP} CXX)

file(GLOB_RECURSE AGENT_APP_SOURCES "src/*.cpp")

add_executable(${AGENT_APP} ${AGENT_APP_SOURCES})

target_include_directories(${AGENT_APP} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Shared-App/include
)


# Вказуємо, що agent_app використовує заголовки з ${SHARED_LIB}
target_link_libraries(${AGENT_APP} 
    PUBLIC SharedApplib
    PRIVATE cpr::cpr
)