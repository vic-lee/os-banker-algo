#ifndef HEADER_TASKTABLE
#define HEADER_TASKTABLE

#include <map>
#include "task.h"

namespace task
{
class TaskTable
{
    std::map<int, Task> task_table;
    void add(Task task);
    void add_new_activity_to_task(Activity activity);

  public:
    void create_task_from_input(std::vector<std::string> parsed_line);
    void add_new_request_to_task(std::vector<std::string> parsed_line);
    void add_new_release_to_task(std::vector<std::string> parsed_line);
    void add_termination_to_task(std::vector<std::string> parsed_line);

    task::Task* access_task_by_id(int id);
    bool is_all_task_terminated();
    int size();

    void print();
};
} // namespace task

#endif