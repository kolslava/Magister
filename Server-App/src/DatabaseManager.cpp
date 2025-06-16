#include <DatabaseManager.hpp>
#include <Logger.hpp>

#include <sqlite_orm/sqlite_orm.h>

// --- Функція для створення структури БД ---
// Вона потрібна для ініціалізації об'єкта storage
inline auto create_storage(const std::string& path) {
    using namespace sqlite_orm;
    return make_storage(path,
        make_table("agents",
            make_column("id", &Agent::id, primary_key().autoincrement()),
            make_column("hostname", &Agent::hostname),
            make_column("os_version", &Agent::os_version),
            make_column("public_key", &Agent::public_key),
            make_column("created_at", &Agent::created_at, default_value(current_timestamp()))
        ),
        make_table("fingerprints",
            make_column("id", &Fingerprint::id, primary_key().autoincrement()),
            make_column("agent_id", &Fingerprint::agent_id),
            make_column("data", &Fingerprint::data),
            make_column("created_at", &Fingerprint::created_at, default_value(current_timestamp()))
        )
        // TODO: В майбутньому тут будуть інші таблиці (fingerprints, alerts)
    );
}

// Тип нашого сховища
using Storage = decltype(create_storage(""));

// --- Клас Impl, який реалізує PIMPL ---
// Він приховує член класу 'storage' від заголовочного файлу
class DatabaseManager::Impl {
public:
    // Конструктор Impl створює БД і синхронізує схему
    Impl(const std::string& dbPath) : storage(create_storage(dbPath)) {
        storage.sync_schema(true); // true для виводу інформації про зміни
        SharedLib::Logger::info("Database schema synchronized at '{}'", dbPath);
    }

    Storage storage;
};


// --- Реалізація методів DatabaseManager ---

// Конструктор викликається лише один раз при першому виклику get()
DatabaseManager::DatabaseManager() 
    : pimpl(std::make_unique<Impl>("AppDatabase.sqlite")) {
    // Вся ініціалізація відбувається в конструкторі Impl
}

// Деструктор має бути тут, де Impl є повним типом
DatabaseManager::~DatabaseManager() = default;


// Реалізація патерну Singleton
DatabaseManager& DatabaseManager::get() {
    static DatabaseManager instance;
    return instance;
}

// Реалізація публічних методів
int DatabaseManager::addAgent(const Agent& agent) {
    // Вся логіка роботи з БД делегується об'єкту storage всередині pimpl
    return pimpl->storage.insert(agent);
}

std::optional<std::string> DatabaseManager::getAgentPublicKey(int agent_id) {
    auto agent = pimpl->storage.get_pointer<Agent>(agent_id);
    if (agent) {
        return agent->public_key;
    }
    return std::nullopt;
}

void DatabaseManager::addFingerprint(const Fingerprint& fp) {
    pimpl->storage.insert(fp);
}