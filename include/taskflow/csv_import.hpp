#pragma once
#include "taskflow/task.hpp"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace taskflow {

// Дополнительно: CSV через запятую (у osipov было через ;)
// title,assignee,priority,description
inline Task task_from_csv(const std::string& line) {
    std::stringstream ss(line);
    std::string part;
    std::vector<std::string> parts;
    while (std::getline(ss, part, ',')) {
        parts.push_back(part);
    }
    if (parts.size() < 3) {
        throw std::invalid_argument("csv needs title,assignee,priority");
    }
    Task t;
    t.title = parts[0];
    t.assignee = parts[1];
    t.priority = std::stoi(parts[2]);
    if (parts.size() >= 4) t.description = parts[3];
    t.status = TaskStatus::New;
    return t;
}

} // namespace taskflow
