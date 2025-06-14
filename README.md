sudo apt-get update
sudo apt-get install -y libsqlite3-dev libboost-system-dev libssl-dev zlib1g-dev libcurl4-openssl-dev

git submodule add https://github.com/gabime/spdlog.git external/spdlog

git submodule add https://github.com/fnc12/sqlite_orm.git external/sqlite_orm

git submodule add https://github.com/nlohmann/json.git external/json

# HTTP Сервер
git submodule add https://github.com/CrowCpp/Crow.git external/crow

# HTTP Клієнт
git submodule add https://github.com/libcpr/cpr.git external/cpr

git submodule update --init --recursive

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Запусти збірку. -j$(nproc) використовує всі доступні ядра для прискорення.
make -j$(nproc)