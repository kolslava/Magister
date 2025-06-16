/*#include <iostream>

#include <Shared.hpp>

#include <cpr/cpr.h>



int main(int argc, char* argv[]) {

    SharedLib::Logger::Init("AgentApp", "./logs/agent.log");

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

    // 1. Ініціалізуємо логер та конфігурацію
    SharedLib::AppConfig config;

    // 2. Формуємо URL до тестового ендпоінта /health
    std::string url = config.getClientTargetUrl("/health");
    SharedLib::Logger::info("Sending request to test server health at: {}", url);

    // 3. Робимо GET-запит
    cpr::Response r = cpr::Get(cpr::Url{url},
                               // Цей прапорець потрібен для самопідписаних сертифікатів
                               cpr::VerifySsl{config.client_verify_ssl});

    // 4. Аналізуємо відповідь
    if (r.status_code == 200) {
        SharedLib::Logger::info("Server response OK (200).");
        SharedLib::Logger::info("Response body: {}", r.text);

        // Спробуємо розпарсити відповідь
        auto status_opt = SharedLib::JsonSerializer::deserializeHealthStatus(r.text);
        if(status_opt) {
            SharedLib::Logger::info("Successfully deserialized health status: status='{}', service='{}'", 
                                 status_opt->status, status_opt->service);
        } else {
            SharedLib::Logger::error("Failed to deserialize health status from server response.");
        }

    } else {
        SharedLib::Logger::error("Request failed! Status code: {}. Error: {}", r.status_code, r.error.message);
    }

    std::cout << "Hello, world from AgentAPP\n";

    return 0;
}*/


#include <Shared.hpp>

#include <iostream>

// Підключаємо заголовки для функцій ініціалізації OpenSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

/**
 * @brief Виконує одноразову ініціалізацію бібліотеки OpenSSL.
 * Має бути викликана на самому початку роботи програми.
 */
void InitializeOpenSSL() {
    SSL_load_error_strings();      // Завантажуємо рядки помилок
    OpenSSL_add_ssl_algorithms();  // Реєструємо реалізації шифрів та дайджестів
}

int main() {
    InitializeOpenSSL();
    SharedLib::Logger::Init("CryptoTest", "test.log");

    fmt::print("--- КРОК 1: Генерація ключів ---\n");
    std::string privKey, pubKey;
    if (!SharedLib::CryptoHelper::generateRsaKeyPair(privKey, pubKey)) {
        SharedLib::Logger::critical("Помилка генерації ключів!");
        return 1;
    }
    SharedLib::Logger::info("Пара ключів успішно згенерована.");
    SharedLib::Logger::info("\nПРИВАТНИЙ КЛЮЧ (PEM):\n{}\n", privKey);
    SharedLib::Logger::info("ПУБЛІЧНИЙ КЛЮЧ (PEM):\n{}\n", pubKey);


    SharedLib::Logger::info("\n--- КРОК 2: Підготовка та підпис даних ---\n");
    SharedLib::JsonSerializer::FingerprintData data_to_sign;
    data_to_sign.metrics["hostname"] = "test-pc";
    data_to_sign.metrics["ip"] = "192.168.1.10";
    data_to_sign.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    
    std::string data_string = SharedLib::JsonSerializer::serialize(data_to_sign);
    SharedLib::Logger::info("Дані, які ми будемо підписувати (у форматі JSON):");
    SharedLib::Logger::info("{}\n", data_string);

    std::string signature;
    if (!SharedLib::SignatureManager::sign(privKey, data_string, signature)) {
        SharedLib::Logger::critical("Помилка створення підпису!");
        return 1;
    }
    SharedLib::Logger::info("Підпис успішно створено.");
    SharedLib::Logger::info("\nЦИФРОВИЙ ПІДПИС (у форматі Base64):\n{}\n", signature);


    SharedLib::Logger::info("\n--- КРОК 3: Перевірка підпису (симуляція роботи сервера) ---\n");
    SharedLib::Logger::info("Перевіряємо підпис з правильними даними...");
    bool is_valid = SharedLib::SignatureManager::verify(pubKey, data_string, signature);

    if (is_valid) {
        SharedLib::Logger::info("\n========================================\n");
        SharedLib::Logger::info("УСПІХ: Підпис валідний! Сервер довіряє цим даним.");
        SharedLib::Logger::info("========================================\n");
    } else {
        SharedLib::Logger::info("\n========================================\n");
        SharedLib::Logger::error("ПОМИЛКА: Підпис не валідний!");
        SharedLib::Logger::info("========================================\n");
    }

    return 0;
}