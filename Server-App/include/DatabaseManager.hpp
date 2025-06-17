/*

*/

#pragma once

#define DATABASEMANAGER_HPP
#ifdef DATABASEMANAGER_HPP

//#include <sqlite_orm/sqlite_orm.h>

#include <string>
#include <vector>
#include <optional>
#include <memory>

// --- Структури даних, що відображають таблиці в БД ---
// Ми визначаємо їх тут, щоб вони були доступні для використання в сервері.

// Структури даних, що відображають таблиці в БД
struct Agent {
    int id = -1;
    std::string hostname;
    std::string os_version;
    std::string public_key;
    std::string enrollment_token;
    bool is_active = false;
    std::string created_at;
};

struct Fingerprint {
    int id = -1;
    int agent_id;
    std::string data; // Зберігаємо як JSON-рядок
    std::string created_at;
};

class DatabaseManager {
public:
    static DatabaseManager& get();
    
    // Створює "запрошення" для нового агента
    std::string generateEnrollmentToken(const std::string& description);
    
    // Активує агента за токеном, оновлюючи його дані
    std::optional<int> activateAgent(const std::string& token, const std::string& publicKey, const std::string& hostname, const std::string& osVersion);
    
    // Отримує ключ для перевірки підпису
    std::optional<std::string> getAgentPublicKey(int agent_id);

    // Зберігає новий "відбиток"
    void addFingerprint(const Fingerprint& fp);
    
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;

    DatabaseManager();
    ~DatabaseManager();
};

#endif // DATABASEMANAGER_HPP