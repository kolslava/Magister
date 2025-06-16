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

struct Agent {
    int id = -1;
    std::string hostname;
    std::string os_version;
    std::string public_key;
    std::string created_at;
};

struct Fingerprint {
    int id = -1;
    int agent_id;
    std::string data; // Будемо зберігати як JSON-рядок
    std::string created_at;
};

// TODO: В майбутньому тут будуть інші структури (Fingerprint, Alert і т.д.)

/**
 * @brief Клас-одинак для керування базою даних.
 * Надає єдину точку доступу до БД для всього застосунку.
 */

class DatabaseManager {
public:
    // --- Публічний інтерфейс ---

    /**
     * @brief Отримує єдиний екземпляр менеджера БД.
     */
    static DatabaseManager& get();

    /**
     * @brief Додає нового агента в базу даних.
     * @param agent Об'єкт агента для збереження.
     * @return Унікальний ID, присвоєний агенту в БД.
     */
    int addAgent(const Agent& agent);

    void addFingerprint(const Fingerprint& fp);

    /**
     * @brief Отримує публічний ключ агента за його ID.
     * @param agent_id ID агента.
     * @return Рядок з ключем, або std::nullopt, якщо агента не знайдено.
     */
    std::optional<std::string> getAgentPublicKey(int agent_id);

    // TODO: В майбутньому тут будуть інші методи:
    // void addFingerprint(int agent_id, const std::string& fingerprint);
    // void addAlert(int agent_id, const std::string& alert_message);


    // --- Заборона копіювання та присвоєння для патерну Singleton ---
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    // --- Реалізація PIMPL ---
    class Impl; // Випереджаюча декларація внутрішнього класу
    std::unique_ptr<Impl> pimpl; // Вказівник на реалізацію

    // Конструктор та деструктор приватні для Singleton
    DatabaseManager();
    ~DatabaseManager();
};

#endif // DATABASEMANAGER_HPP