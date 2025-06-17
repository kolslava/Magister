#include <DatabaseManager.hpp>
#include <Logger.hpp>

#include <sqlite_orm/sqlite_orm.h>

#include <random>

// --- Функція для створення структури БД ---
// Вона потрібна для ініціалізації об'єкта storage
// Функція для створення схеми БД з правильним синтаксисом
inline auto create_storage(const std::string& path) {
    using namespace sqlite_orm;
    return make_storage(path,
        make_table("agents",
            make_column("id", &Agent::id, primary_key().autoincrement()), // ВИПРАВЛЕНО: autoincrement() ПЕРЕД primary_key()
            make_column("hostname", &Agent::hostname),
            make_column("os_version", &Agent::os_version),
            make_column("public_key", &Agent::public_key, default_value("")),
            make_column("enrollment_token", &Agent::enrollment_token, unique()),
            make_column("is_active", &Agent::is_active, default_value(false)),
            make_column("created_at", &Agent::created_at, default_value(current_timestamp()))
        ),
        make_table("fingerprints",
            make_column("id", &Fingerprint::id, primary_key().autoincrement()),
            make_column("agent_id", &Fingerprint::agent_id),
            make_column("data", &Fingerprint::data),
            make_column("created_at", &Fingerprint::created_at, default_value(current_timestamp()))
        )
    );
}

// Тип нашого сховища
using Storage = decltype(create_storage(""));

// --- Клас Impl, який реалізує PIMPL ---
// Він приховує член класу 'storage' від заголовочного файлу
class DatabaseManager::Impl {
public:
    Impl(const std::string& dbPath) : storage(create_storage(dbPath)) {
        storage.sync_schema(true);
    }
    Storage storage;
};

// --- Реалізація методів DatabaseManager ---
DatabaseManager::DatabaseManager() : pimpl(std::make_unique<Impl>("AppDatabase.sqlite")) {}
DatabaseManager::~DatabaseManager() = default;
DatabaseManager& DatabaseManager::get() {
    static DatabaseManager instance;
    return instance;
}

std::string generate_random_token() {
    std::stringstream ss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 0xFFFF);
    for(int i = 0; i < 4; ++i) { ss << std::hex << distrib(gen); }
    return ss.str();
}

std::string DatabaseManager::generateEnrollmentToken(const std::string& description) {
    std::string token = generate_random_token();
    Agent placeholder;
    placeholder.hostname = description;
    placeholder.os_version = "pending";
    placeholder.public_key = "";
    placeholder.enrollment_token = token;
    placeholder.is_active = false;
    pimpl->storage.insert(placeholder);
    SharedLib::Logger::info("Generated enrollment token '{}' for '{}'", token, description);
    return token;
}

std::optional<int> DatabaseManager::activateAgent(const std::string& token, const std::string& publicKey, const std::string& hostname, const std::string& osVersion) {
    using namespace sqlite_orm;
    SharedLib::Logger::info("Attempting to activate agent with token: {}", token);

    auto agentVec = pimpl->storage.get_all<Agent>(where(c(&Agent::enrollment_token) == token && c(&Agent::is_active) == false));
    
    if (!agentVec.empty()) {
        auto agent = agentVec.front(); // Беремо першого знайденого
        SharedLib::Logger::info("Found placeholder for agent '{}'. Activating...", agent.hostname);
        
        agent.hostname = hostname;
        agent.os_version = osVersion;
        agent.public_key = publicKey;
        agent.is_active = true;
        agent.enrollment_token = "USED_" + token;
        pimpl->storage.update(agent);
        
        return agent.id;
    }
    
    SharedLib::Logger::warn("No inactive agent found for the provided token.");
    return std::nullopt;
}

std::optional<std::string> DatabaseManager::getAgentPublicKey(int agent_id) {
    if(auto agent = pimpl->storage.get_pointer<Agent>(agent_id)) {
        return agent->public_key;
    }
    return std::nullopt;
}

void DatabaseManager::addFingerprint(const Fingerprint& fp) {
    pimpl->storage.insert(fp);
}