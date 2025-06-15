#include <iostream>

#include <Shared.hpp>

#include <cpr/cpr.h>

int main(int argc, char* argv[]) {

    SharedLib::Logger::Init("AgentApp", "agent.log");

    // --- Тестові повідомлення ---
    SharedLib::Logger::trace("This is a trace message.");
    SharedLib::Logger::info("Server is starting...");
    SharedLib::Logger::warn("This is a warning.");
    SharedLib::Logger::error("This is an error message!");
    SharedLib::Logger::critical("This is a critical error!");
    // -------------------------

    std::cout << "Hello, world from AgentAPP\n";

    return 0;
}