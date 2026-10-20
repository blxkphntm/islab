#include "taskflow/priority_strategy.hpp"

namespace taskflow {

int DefaultPriorityStrategy::score(const Task& task) const {
    return 10 - task.priority;
}

int ClosedLastPriorityStrategy::score(const Task& task) const {
    return is_closed(task.status) ? 1000 : 10 - task.priority;
}

int UrgencyPriorityStrategy::score(const Task& task) const {
    if (is_closed(task.status)) return 2000;
    // 1 и 2 — срочные, идут раньше
    if (task.priority <= 2) return task.priority;
    return 10 + task.priority;
}

} // namespace taskflow
