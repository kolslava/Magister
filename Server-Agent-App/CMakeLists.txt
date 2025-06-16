set(AGENT_APP ServerAgentApp)

set(AGENT_APP_SOURCES
    src/main.cpp
)

add_executable(${AGENT_APP} ${AGENT_APP_SOURCES})

target_link_libraries(${AGENT_APP} 
    PUBLIC SharedApplib
    #PRIVATE cpr::cpr
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC Threads::Threads
)