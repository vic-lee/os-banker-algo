#ifndef HEADER_TASKTABLE
#define HEADER_TASKTABLE

#include <map>
#include <tuple>
#include "task.h"

namespace task
{
class TaskTable
{
    /**
     * This class stores all the tasks in the system. To perform actions on 
     * tasks or activities associated with a task, the algorithm (FIFO or Banker) 
     * talks to the TaskTable first. The table then delegates the thing to do to 
     * the correct Task (identified through the tasks' IDs).
     */

    std::map<int, Task> task_table_;
    void add(Task task);
    bool has_task_been_created(int id);
    void add_new_activity_to_task(Activity *activity);

public:
    void handle_new_initiate(std::vector<std::string> parsed_line);
    void add_new_request_to_task(std::vector<std::string> parsed_line);
    void add_new_release_to_task(std::vector<std::string> parsed_line);
    void add_termination_to_task(std::vector<std::string> parsed_line);

    std::vector<std::vector<int>> generate_unmet_demand_matrix();

    task::Task *access_task_by_id(int id);
    bool is_all_task_terminated();
    int size();

    void print();
};
} // namespace task

#endif