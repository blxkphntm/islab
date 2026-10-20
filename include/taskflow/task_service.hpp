#pragma once
#include "taskflow/priority_strategy.hpp"
#include "taskflow/repository.hpp"
#include <functional>
#include <string>
#include <vector>

namespace taskflow {

using CreateHook = std::function<void(const Task&)>;

class TaskService {
public:
    TaskService(TaskRepositoryPtr repository, PriorityStrategyPtr priority_strategy);

    Task create_task(std::string title, std::string assignee, int priority,
                     std::string description = "", std::string comment = "");
    bool assign(std::int64_t id, std::string assignee);
    bool transition(std::int64_t id, TaskStatus next);
    std::vector<Task> list_by_priority() const;
    std::optional<Task> find(std::int64_t id) const;

    // ЛР2 Observer: колбэк на создание
    void on_created(CreateHook hook);

private:
    void ensure_valid(const std::string& title, const std::string& assignee,
                      int priority, const std::string& description) const;
    bool transition_ok(TaskStatus from, TaskStatus to) const;

    TaskRepositoryPtr repo_;
    PriorityStrategyPtr strategy_;
    std::vector<CreateHook> hooks_;
};

} // namespace taskflow
