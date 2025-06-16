#include <iostream>

#include <Shared.hpp>

#include <DatabaseManager.hpp>
#include <WebServer.hpp>

int main(int argc, char* argv[]) {

    SharedLib::Logger::Init("ServerApp", "./logs/server.log");

    // --- Тестові повідомлення ---
    SharedLib::Logger::trace("This is a trace message.");
    SharedLib::Logger::info("Server is starting...");
    SharedLib::Logger::warn("This is a warning.");
    SharedLib::Logger::error("This is an error message!");
    SharedLib::Logger::critical("This is a critical error!");
    // -------------------------

    // Створюємо екземпляр нашої вкладеної структури
    SharedLib::JsonSerializer::Test original_data;
    original_data.name = "Це мій перший тестовий об'єкт!";

    SharedLib::Logger::info("Original object data: name = '{}'", original_data.name);

    // Викликаємо наш новий статичний метод для серіалізації
    std::string json_string = SharedLib::JsonSerializer::serialize(original_data);

    SharedLib::Logger::info("Serialized to JSON string:\n{}", json_string);


    // --- 2. Десеріалізація (Успішний випадок) ---
    SharedLib::Logger::info("\n--- Starting Deserialization Test (Success Case) ---");

    // Викликаємо метод для десеріалізації
    auto deserialized_opt = SharedLib::JsonSerializer::deserializeTest(json_string);

    // Перевіряємо, чи вдалося розпарсити дані
    if (deserialized_opt) {
        // Якщо так, отримуємо доступ до об'єкта
        SharedLib::JsonSerializer::Test deserialized_data = *deserialized_opt;
        SharedLib::Logger::info("Deserialization successful!");
        SharedLib::Logger::info("Deserialized object data: name = '{}'", deserialized_data.name);

        // Перевіряємо, чи дані збігаються
        if (original_data.name == deserialized_data.name) {
            SharedLib::Logger::info("SUCCESS: Original and deserialized data match.");
        } else {
            SharedLib::Logger::error("FAILURE: Data does not match after deserialization!");
        }
    } else {
        SharedLib::Logger::error("FAILURE: Deserialization failed when it should have succeeded!");
    }


    // --- 3. Десеріалізація (Випадок з помилкою) ---
    SharedLib::Logger::info("\n--- Starting Deserialization Test (Failure Case) ---");
    std::string bad_json_string = R"({"wrong_key": "some value"})"; // JSON без поля "name"

    SharedLib::Logger::info("Attempting to deserialize malformed JSON: {}", bad_json_string);
    auto failed_deserialization_opt = SharedLib::JsonSerializer::deserializeTest(bad_json_string);

    // Перевіряємо, що десеріалізація НЕ вдалася
    if (!failed_deserialization_opt) {
        SharedLib::Logger::info("SUCCESS: Deserialization correctly failed as expected.");
    } else {
        SharedLib::Logger::error("FAILURE: Deserialization succeeded when it should have failed!");
    }

    std::cout << "Hello, world from ServerAPP\n";

    auto& db = DatabaseManager::get();

    // Створюємо об'єкт для збереження в БД
    Agent agent_to_db{
        -1,
        "Test_HOSTNAME",
        "OS_VERsion",
        "PUBLIC_KEY"
    };

    int new_id = db.addAgent(agent_to_db);

    try {
        // 1. Створюємо конфігурацію
        SharedLib::AppConfig config;

        // 2. Створюємо екземпляр сервера з налаштуваннями
        WebServer server(
            config.server_port, 
            config.server_cert_path, 
            config.server_key_path
        );

        // 3. Запускаємо сервер
        server.run();

    } catch (const std::exception& e) {
        //std::cerr << "A critical error occurred: " << e.what() << std::endl;
        // Можна також використати логер, якщо він встиг ініціалізуватися
        SharedLib::Logger::critical("A critical error occurred: {}", e.what());
        return 1;
    }

    return 0;
}