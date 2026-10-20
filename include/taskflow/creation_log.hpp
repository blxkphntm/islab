#pragma once
#include "taskflow/task.hpp"
#include <string>
#include <vector>

namespace taskflow {

// ЛР2: журнал только id созданных задач
class CreationLog {
public:
    void remember(const Task& task) { ids_.push_back(task.id); }
    const std::vector<std::int64_t>& ids() const { return ids_; }
    std::size_t size() const { return ids_.size(); }

private:
    std::vector<std::int64_t> ids_;
};

} // namespace taskflow
