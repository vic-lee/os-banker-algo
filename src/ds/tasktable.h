#ifndef HEADER_TASKTABLE
#define HEADER_TASKTABLE

#include <map>
#include <tuple>
#include "task.h"

namespace task
{
class TaskTable
{
    std::map<int, Task> task_table;
    void add(Task task);
    bool has_task_been_created(int id);
    void add_new_activity_to_task(Activity *activity);


    void do_all_latest_activity_of_type(std::string type, std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle);

  public:
    void handle_new_initiate(std::vector<std::string> parsed_line);
    void add_new_request_to_task(std::vector<std::string> parsed_line);
    void add_new_release_to_task(std::vector<std::string> parsed_line);
    void add_termination_to_task(std::vector<std::string> parsed_line);
    std::tuple<Task *, int> get_next_active_task(int prior_id);
    void iterate_task_activities(bool been_deadlocked, ResourceTable *resource_table, int cycle);

    std::map<int, bool> create_visit_status_table_for_all_tasks();
    
    void do_all_latest_initiates(std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle);
    void do_all_latest_terminates(std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle);
    void do_all_latest_requests(std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle);
    void do_all_latest_releases(std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle);

    task::Task *access_task_by_id(int id);
    bool is_all_task_terminated();
    int size();

    void print();
};
} // namespace task

#endif