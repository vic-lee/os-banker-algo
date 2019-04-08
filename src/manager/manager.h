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
    Manager(task::TaskTable task_table, task::ResourceTable resource_table);
    virtual ~Manager();
    virtual void do_tasks();
    virtual void print();

  protected:
    int cycle_;
    task::TaskTable task_table_;
    std::vector<task::Task *> blocked_tasks_table_;
    task::ResourceTable resource_table_;

    bool is_in_blocked_table(int id);
    void remove_from_blocked_table(task::Task *t);

    std::map<int, bool> create_visit_status_table_for_all_tasks();

    void do_all_latest_initiates(std::map<int, bool> &visit_status);
    void do_all_latest_terminates(std::map<int, bool> &visit_status);

    void do_all_latest_requests(std::map<int, bool> &visit_status);
    void do_all_blocked_tasks_with_requests(std::map<int, bool> &visit_status);
    void do_all_non_blocked_tasks_with_requests(std::map<int, bool> &visit_status);

    void do_all_latest_releases(std::map<int, bool> &visit_status);
    void do_all_latest_activity_of_type(std::string type, std::map<int, bool> &visit_status);
    bool do_one_latest_activity_of_type(std::string type, std::map<int, bool> &visit_status, task::Task *task, bool from_blocked);
};
} // namespace manager

#endif