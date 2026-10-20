#include "taskflow/creation_log.hpp"
#include "taskflow/csv_import.hpp"
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/priority_strategy.hpp"
#include "taskflow/task_service.hpp"
#include <iostream>
#include <memory>

int main() {
    using namespace taskflow;
    auto repo = std::make_shared<InMemoryTaskRepository>();
    auto strategy = std::make_shared<UrgencyPriorityStrategy>();
    TaskService service(repo, strategy);

    CreationLog log;
    service.on_created([&](const Task& t) { log.remember(t); });

    service.create_task("Plan work", "Kuznetsov", 2, "план", "ok");
    service.create_task("Write code", "Kuznetsov", 1, "сервис", "wip");
    auto imported = task_from_csv("Run tests,QA,4,ctest run");
    imported = repo->create(imported);

    std::cout << "Kyznetsov TaskFlow\n";
    for (const auto& t : service.list_by_priority()) {
        std::cout << t.id << ". " << t.title << " | " << t.assignee
                  << " | " << to_string(t.status) << " | p=" << t.priority
                  << " | " << t.description;
        if (!t.comment.empty()) std::cout << " (" << t.comment << ")";
        std::cout << '\n';
    }
    std::cout << "created ids: " << log.size()
              << ", csv id=" << imported.id << '\n';
    return 0;
}
