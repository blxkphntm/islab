#include "simple_test.hpp"
#include "taskflow/creation_log.hpp"
#include "taskflow/csv_import.hpp"
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/priority_strategy.hpp"
#include "taskflow/repository_factory.hpp"
#include "taskflow/sqlite_task_repository.hpp"
#include "taskflow/task_service.hpp"
#include <memory>
#include <string>

using namespace taskflow;

TEST_CASE("factory memory") {
    Config cfg; cfg.kind = "memory";
    auto repo = make_repository(cfg);
    Task d; d.title = "a"; d.assignee = "b"; d.priority = 3;
    CHECK_TRUE(repo->find_by_id(repo->create(d).id).has_value());
}

TEST_CASE("closed last") {
    auto repo = std::make_shared<InMemoryTaskRepository>();
    TaskService s(repo, std::make_shared<ClosedLastPriorityStrategy>());
    auto a = s.create_task("A", "u", 1);
    auto b = s.create_task("B", "u", 5);
    s.transition(a.id, TaskStatus::InProgress);
    s.transition(a.id, TaskStatus::Done);
    CHECK_EQ(s.list_by_priority().front().id, b.id);
}

TEST_CASE("factory sqlite") {
    Config cfg; cfg.kind = "sqlite"; cfg.sqlite_name = "k.tmp.db";
    auto repo = make_repository(cfg);
    auto* p = dynamic_cast<SQLiteTaskRepository*>(repo.get());
    CHECK_TRUE(p != nullptr);
    CHECK_EQ(p->name(), std::string("k.tmp.db"));
}

TEST_CASE("factory bad kind") {
    Config cfg; cfg.kind = "redis";
    CHECK_THROWS(make_repository(cfg));
}

TEST_CASE("urgency strategy") {
    auto repo = std::make_shared<InMemoryTaskRepository>();
    TaskService s(repo, std::make_shared<UrgencyPriorityStrategy>());
    auto normal = s.create_task("N", "u", 4);
    auto urgent = s.create_task("U", "u", 1);
    CHECK_EQ(s.list_by_priority().front().id, urgent.id);
    CHECK_EQ(s.list_by_priority().back().id, normal.id);
}

TEST_CASE("creation log observer") {
    TaskService s(std::make_shared<InMemoryTaskRepository>(),
                  std::make_shared<DefaultPriorityStrategy>());
    CreationLog log;
    s.on_created([&](const Task& t) { log.remember(t); });
    auto t = s.create_task("X", "K", 2);
    CHECK_TRUE(log.size() == 1);
    CHECK_EQ(log.ids().front(), t.id);
}

TEST_CASE("csv comma import") {
    auto t = task_from_csv("Hello,World,2,desc");
    CHECK_EQ(t.title, std::string("Hello"));
    CHECK_EQ(t.assignee, std::string("World"));
    CHECK_EQ(t.priority, 2);
}

TEST_CASE("csv bad") { CHECK_THROWS(task_from_csv("only")); }

TEST_CASE("sqlite stub") {
    SQLiteTaskRepository r("z.db");
    CHECK_THROWS(r.list());
}
