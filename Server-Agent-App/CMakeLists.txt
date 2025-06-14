set(AGENT_APP ServerAgentApp)

set(AGENT_APP_SOURCES
    src/main.cpp
)

add_executable(${AGENT_APP} ${AGENT_APP_SOURCES})

#target_include_directories(${AGENT_APP} PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/Shared-App/include
#)

target_link_libraries(${AGENT_APP} 
    PUBLIC SharedApplib
    PRIVATE cpr::cpr
)