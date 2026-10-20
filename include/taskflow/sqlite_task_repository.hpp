#pragma once
#include "taskflow/repository.hpp"
#include <stdexcept>
#include <string>

namespace taskflow {

class SQLiteTaskRepository final : public ITaskRepository {
public:
    explicit SQLiteTaskRepository(std::string name) : name_(std::move(name)) {}

    Task create(Task) override { fail(); }
    std::optional<Task> find_by_id(std::int64_t) const override { fail(); }
    std::vector<Task> list() const override { fail(); }
    bool update(const Task&) override { fail(); }

    const std::string& name() const { return name_; }

private:
    std::string name_;
    [[noreturn]] void fail() const {
        throw std::runtime_error("sqlite stub: " + name_);
    }
};

} // namespace taskflow
