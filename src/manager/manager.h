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

    /**
     * The Manager class is a base class for the FIFO and Banker algorithm. 
     * It encapsulates shared functionalities and data members for the two
     * algorithms. 
     */

public:
    Manager(task::TaskTable task_table, task::ResourceTable resource_table, bool debug);
    virtual ~Manager();
    virtual void iterate_cycle();
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

    bool should_visit_task(task::Task *t);

    bool do_task_latest_activity(task::Task *t);

    bool has_visited(task::Task *t);
    void mark_as_visited(task::Task *t);

    void incr_blocked_task_waiting_time();
    void decr_delay_countdowns();

    std::map<int, bool> create_visit_status_table_for_all_tasks();

    bool is_request_safe(task::Task *task);

    void do_latest_initiates();
    void do_latest_terminates();
    void do_latest_releases();
    virtual void do_latest_requests();
};
} // namespace manager

#endif