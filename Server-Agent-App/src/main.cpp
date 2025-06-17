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


/*#include <Shared.hpp>

#include <iostream>

// Підключаємо заголовки для функцій ініціалізації OpenSSL
#include <openssl/ssl.h>
#include <openssl/err.h>*/

/**
 * @brief Виконує одноразову ініціалізацію бібліотеки OpenSSL.
 * Має бути викликана на самому початку роботи програми.
 */

/*
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
}*/

#include <Shared.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include <cpr/cpr.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

const std::string KEY_PUBLIC_PATH = "./agent-keys/public.key";
const std::string KEY_PRIVATE_PATH = "./agent-keys/private.key";
const std::string AGENT_CONFIG_PATH = "./agent-conf/agent.conf";

/**
 * @brief Виконує одноразову ініціалізацію бібліотеки OpenSSL.
 */
void InitializeOpenSSL() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

/**
 * @brief Допоміжна функція для збереження даних у файл.
 */
bool save_to_file(const std::string& path, const std::string& data) {
    std::ofstream out(path);
    if (!out) {
        SharedLib::Logger::error("Failed to open file for writing: {}", path);
        return false;
    }
    out << data;
    return true;
}

/**
 * @brief Допоміжна функція для читання даних з файлу.
 */
bool load_from_file(const std::string& path, std::string& data) {
    std::ifstream in(path);
    if (!in) {
        return false; // Це нормально, якщо файл ще не існує
    }
    data.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return true;
}


int main() {
    InitializeOpenSSL();
    SharedLib::Logger::Init("AgentApp", "./logs/agent.log");

    /*try {
        SharedLib::AppConfig config;
        long long agent_id = -1;
        std::string privateKey;

        std::string agent_id_str;
        if (load_from_file(AGENT_CONFIG_PATH, agent_id_str) && !agent_id_str.empty()) {
            try {
                agent_id = std::stoll(agent_id_str);
                SharedLib::Logger::info("Agent is already registered with ID: {}", agent_id);
                if (!load_from_file(KEY_PRIVATE_PATH, privateKey)) {
                    SharedLib::Logger::critical("Config file found, but private key '{}' is missing!", KEY_PRIVATE_PATH);
                    return 1;
                }
            } catch(...) { agent_id = -1; }
        }

        if (agent_id == -1) {
            SharedLib::Logger::warn("Agent is not registered. Starting registration process...");

            std::string publicKey;
            if (!SharedLib::CryptoHelper::generateRsaKeyPair(privateKey, publicKey)) {
                SharedLib::Logger::critical("Could not generate RSA key pair!");
                return 1;
            }
            if (!save_to_file(KEY_PRIVATE_PATH, privateKey) || !save_to_file(KEY_PUBLIC_PATH, publicKey)) {
                SharedLib::Logger::critical("Could not save key files!");
                return 1;
            }
            SharedLib::Logger::info("New key pair generated and saved.");
            
            SharedLib::JsonSerializer::AgentRegisterRequest request;
            request.hostname = "agent-007"; // TODO: get real hostname
            request.os_version = "Linux 22.04";  // TODO: get real OS version
            request.public_key_pem = publicKey;
            
            std::string url = config.getClientTargetUrl("/api/v1/agents");
            SharedLib::Logger::info("Sending registration request to {}", url);
            
            cpr::Response r = cpr::Post(cpr::Url{url},
                                        cpr::Body{SharedLib::JsonSerializer::serialize(request)},
                                        cpr::Header{{"Content-Type", "application/json"}},
                                        cpr::VerifySsl{config.client_verify_ssl});

            if (r.status_code == 200) {
                auto responseOpt = SharedLib::JsonSerializer::deserializeRegisterResponse(r.text);
                if (!responseOpt) {
                    SharedLib::Logger::critical("Failed to parse registration response from server!");
                    return 1;
                }
                agent_id = responseOpt->agent_id;
                if (!save_to_file(AGENT_CONFIG_PATH, std::to_string(agent_id))) {
                    SharedLib::Logger::critical("Could not save agent config file '{}'!", AGENT_CONFIG_PATH);
                    return 1;
                }
                SharedLib::Logger::info("Registration successful! Received Agent ID: {}. Saved to '{}'", agent_id, AGENT_CONFIG_PATH);
            } else {
                SharedLib::Logger::critical("Registration request failed! Status: {}, Body: {}", r.status_code, r.text);
                return 1;
            }
        }

        SharedLib::Logger::info("Agent is running. Agent ID: {}. Starting main work loop...", agent_id);

        while (true) {
            SharedLib::Logger::info("--- Collecting and sending fingerprint ---");

            // 1. Готуємо дані
            SharedLib::JsonSerializer::FingerprintData data_to_sign;
            data_to_sign.metrics["hostname"] = "agent-007";
            data_to_sign.metrics["uptime_seconds"] = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count() / 1000000000);
            data_to_sign.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        
            std::string data_string = SharedLib::JsonSerializer::serialize(data_to_sign);

            // 2. Підписуємо дані
            std::string signature;
            if (!SharedLib::SignatureManager::sign(privateKey, data_string, signature)) {
                SharedLib::Logger::error("Could not sign data. Skipping this cycle.");
                std::this_thread::sleep_for(std::chrono::seconds(15));
                continue;
            }
        
            // 3. Формуємо та відправляємо запит
            SharedLib::JsonSerializer::FingerprintSubmitRequest request;
            request.data = data_to_sign;
            request.signature_base64 = signature;

            std::string url = config.getClientTargetUrl("/api/v1/agents/" + std::to_string(agent_id) + "/fingerprints");
            cpr::Response r = cpr::Post(cpr::Url{url},
                                        cpr::Body{SharedLib::JsonSerializer::serialize(request)},
                                        cpr::Header{{"Content-Type", "application/json"}},
                                        cpr::VerifySsl{config.client_verify_ssl});

            if (r.status_code == 200) {
                SharedLib::Logger::info("Fingerprint successfully submitted.");
            } else {
                SharedLib::Logger::error("Fingerprint submission failed! Status: {}, Body: {}", r.status_code, r.text);
            }

            SharedLib::Logger::info("--- Cycle finished. Waiting for 15 seconds... ---");
            std::this_thread::sleep_for(std::chrono::seconds(15));
        }
    } catch (const std::exception& e) {
        SharedLib::Logger::critical("A critical error occurred in AgentApp: {}", e.what());
        return 1;
    }
    return 0;*/

    try {
        SharedLib::AppConfig app_config;
        long long agent_id = -1;
        std::string privateKey;

        // --- ЕТАП 1: ЧИТАННЯ КОНФІГУРАЦІЇ ---
        std::string config_content;
        load_from_file(AGENT_CONFIG_PATH, config_content);
        
        auto agent_config_opt = SharedLib::JsonSerializer::deserializeAgentConfig(config_content);
        if (!agent_config_opt) {
            SharedLib::Logger::critical("Failed to parse config file '{}'. Please fix or delete it.", AGENT_CONFIG_PATH);
            return 1;
        }
        auto agent_config = *agent_config_opt;
        
        // --- ЕТАП 2: ПЕРЕВІРКА РЕЄСТРАЦІЇ ---
        if (agent_config.agent_id) {
            agent_id = *agent_config.agent_id;
            SharedLib::Logger::info("Agent is already registered with ID: {}", agent_id);
            if (!load_from_file(KEY_PRIVATE_PATH, privateKey)) {
                 SharedLib::Logger::critical("Config file found, but private key '{}' is missing!", KEY_PRIVATE_PATH);
                 return 1;
            }
        } else {
            // --- ЕТАП 3: РЕЄСТРАЦІЯ ЗА ТОКЕНОМ ---
            SharedLib::Logger::warn("Agent not registered. Looking for enrollment token...");
            
            if (!agent_config.enrollment_token) {
                SharedLib::Logger::critical("Enrollment token not found in '{}'. Cannot register.", AGENT_CONFIG_PATH);
                return 1;
            }
            std::string token = *agent_config.enrollment_token;

            std::string publicKey;
            if (!SharedLib::CryptoHelper::generateRsaKeyPair(privateKey, publicKey)) {
                SharedLib::Logger::critical("Could not generate RSA key pair!"); return 1;
            }
            if (!save_to_file(KEY_PRIVATE_PATH, privateKey) || !save_to_file(KEY_PUBLIC_PATH, publicKey)) {
                 SharedLib::Logger::critical("Could not save key files!"); return 1;
            }
            SharedLib::Logger::info("New key pair generated and saved.");
            
            SharedLib::JsonSerializer::AgentRegisterRequest request;
            request.hostname = "agent-007";
            request.os_version = "Linux 22.04";
            request.public_key_pem = publicKey;
            
            std::string url = app_config.getClientTargetUrl("/api/v1/register");
            cpr::Header headers = {
                {"Content-Type", "application/json"},
                {"Authorization", "Bearer " + token}
            };
            
            SharedLib::Logger::info("Sending registration request to {}", url);
            cpr::Response r = cpr::Post(cpr::Url{url},
                                        cpr::Body{SharedLib::JsonSerializer::serialize(request)},
                                        headers,
                                        cpr::VerifySsl{app_config.client_verify_ssl});

            if (r.status_code == 200) {
                auto responseOpt = SharedLib::JsonSerializer::deserializeRegisterResponse(r.text);
                if (!responseOpt) {
                    SharedLib::Logger::critical("Failed to parse registration response!"); return 1;
                }
                agent_id = responseOpt->agent_id;
                
                SharedLib::JsonSerializer::AgentConfig new_config;
                new_config.agent_id = agent_id;
                if (!save_to_file(AGENT_CONFIG_PATH, SharedLib::JsonSerializer::serialize(new_config))) {
                    SharedLib::Logger::critical("Could not save agent config file '{}'!", AGENT_CONFIG_PATH);
                    return 1;
                }
                SharedLib::Logger::info("Registration successful! Received Agent ID: {}. Config updated.", agent_id);
            } else {
                SharedLib::Logger::critical("Registration request failed! Status: {}, Body: {}", r.status_code, r.text);
                return 1;
            }
        }

        // --- ЕТАП 4: ОСНОВНИЙ ЦИКЛ РОБОТИ ---
        SharedLib::Logger::info("Agent is running. Agent ID: {}. Main loop started.", agent_id);
        // ...

    } catch (const std::exception& e) {
        SharedLib::Logger::critical("A critical error occurred in AgentApp: {}", e.what());
        return 1;
    }
    return 0;
}