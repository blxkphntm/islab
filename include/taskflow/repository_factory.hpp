#pragma once
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/sqlite_task_repository.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace taskflow {

struct Config {
    std::string kind{"memory"}; // memory | sqlite
    std::string sqlite_name{"kyznetsov.db"};
};

inline TaskRepositoryPtr make_repository(const Config& cfg) {
    if (cfg.kind == "memory") {
        return std::make_shared<InMemoryTaskRepository>();
    }
    if (cfg.kind == "sqlite") {
        return std::make_shared<SQLiteTaskRepository>(cfg.sqlite_name);
    }
    throw std::invalid_argument("unknown kind: " + cfg.kind);
}

} // namespace taskflow
