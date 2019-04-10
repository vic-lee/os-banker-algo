#ifndef HEADER_MANAGER
#define HEADER_MANAGER

#include <vector>
#include "../ds/task.h"
#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace manager
{

class Manager
{
public:
    Manager(task::TaskTable task_table, task::ResourceTable resource_table, bool debug);
    virtual ~Manager();
    virtual void do_tasks();
    virtual void print();

protected:
    bool debug_;
    int cycle_;
    task::TaskTable task_table_;
    std::vector<task::Task *> blocked_tasks_table_;
    task::ResourceTable resource_table_;
    std::map<int, bool> visit_table_;
    bool should_check_safety_;

    bool is_in_blocked_table(int id);
    void block(task::Task *t);
    void remove_from_blocked_table(task::Task *t);

    void before_cycle_setup();
    void after_cycle_teardown();

    void incr_blocked_task_waiting_time();
    void decr_delay_countdowns();

    std::map<int, bool> create_visit_status_table_for_all_tasks();

    void do_all_latest_requests(std::map<int, bool> &visit_status);
    void do_latest_requests_from_blocked_tasks(std::map<int, bool> &visit_status);
    void do_latest_requests_from_non_blocked_tasks(std::map<int, bool> &visit_status);

    bool is_request_safe(task::Task *task);

    void do_all_latest_initiates(std::map<int, bool> &visit_status);
    void do_all_latest_terminates(std::map<int, bool> &visit_status);
    void do_all_latest_releases(std::map<int, bool> &visit_status);
    void do_all_latest_activity_of_type(std::string type, std::map<int, bool> &visit_status);
    bool do_one_latest_activity_of_type(std::string type, std::map<int, bool> &visit_status, task::Task *task, bool from_blocked);
};
} // namespace manager

#endif