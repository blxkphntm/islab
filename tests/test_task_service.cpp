#include "simple_test.hpp"
#include "taskflow/creation_log.hpp"
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/priority_strategy.hpp"
#include "taskflow/task_service.hpp"
#include <memory>
#include <string>

using namespace taskflow;

static TaskService make() {
    return TaskService(std::make_shared<InMemoryTaskRepository>(),
                       std::make_shared<DefaultPriorityStrategy>());
}

TEST_CASE("create ok") {
    auto s = make();
    auto t = s.create_task("T", "K", 2, "d", "c");
    CHECK_TRUE(t.id > 0);
    CHECK_EQ(t.comment, std::string("c"));
}

TEST_CASE("bad priority") { CHECK_THROWS(make().create_task("T", "K", 9)); }

TEST_CASE("workflow") {
    auto s = make();
    auto t = s.create_task("T", "K", 1);
    CHECK_TRUE(s.transition(t.id, TaskStatus::InProgress));
    CHECK_TRUE(s.transition(t.id, TaskStatus::Done));
    CHECK_TRUE(!s.transition(t.id, TaskStatus::InProgress));
}

TEST_CASE("assign ok") {
    auto s = make();
    auto t = s.create_task("T", "A", 2);
    CHECK_TRUE(s.assign(t.id, "B"));
    CHECK_EQ(s.find(t.id)->assignee, std::string("B"));
}

TEST_CASE("assign fail Done") {
    auto s = make();
    auto t = s.create_task("T", "A", 2);
    s.transition(t.id, TaskStatus::InProgress);
    s.transition(t.id, TaskStatus::Done);
    CHECK_TRUE(!s.assign(t.id, "B"));
}

TEST_CASE("desc too long") {
    CHECK_THROWS(make().create_task("T", "A", 2, std::string(1001, 'x')));
}

TEST_CASE("desc 1000 ok") {
    auto t = make().create_task("T", "A", 2, std::string(1000, 'x'));
    CHECK_EQ(t.description.size(), static_cast<std::size_t>(1000));
}

TEST_CASE("empty title") { CHECK_THROWS(make().create_task("", "A", 2)); }
TEST_CASE("empty assignee") { CHECK_THROWS(make().create_task("T", "", 2)); }
TEST_CASE("prio 0") { CHECK_THROWS(make().create_task("T", "A", 0)); }
TEST_CASE("prio 6") { CHECK_THROWS(make().create_task("T", "A", 6)); }

TEST_CASE("cancel ok") {
    auto s = make();
    auto t = s.create_task("T", "A", 3);
    CHECK_TRUE(s.transition(t.id, TaskStatus::Cancelled));
}

TEST_CASE("assign empty") {
    auto s = make();
    auto t = s.create_task("T", "A", 3);
    CHECK_THROWS(s.assign(t.id, ""));
}

TEST_CASE("no id") { CHECK_TRUE(!make().find(77).has_value()); }

TEST_CASE("repo integration") {
    auto repo = std::make_shared<InMemoryTaskRepository>();
    Task d; d.title = "x"; d.assignee = "y"; d.priority = 2;
    auto c = repo->create(d);
    CHECK_TRUE(repo->find_by_id(c.id).has_value());
    c.status = TaskStatus::InProgress;
    CHECK_TRUE(repo->update(c));
    CHECK_TRUE(repo->list().size() == 1);
}

TEST_CASE("default order") {
    auto s = make();
    auto low = s.create_task("L", "a", 5);
    auto high = s.create_task("H", "b", 1);
    auto o = s.list_by_priority();
    CHECK_EQ(o.front().id, low.id);
    CHECK_EQ(o.back().id, high.id);
}
