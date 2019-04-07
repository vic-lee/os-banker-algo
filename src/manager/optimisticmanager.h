#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace manager
{

class OptimisticManager
{
    int cycle_;
    task::TaskTable task_table;
    task::ResourceTable resource_table_;

    bool does_deadlock_exist(std::map<int, bool> visit_status);
    void handle_deadlock();
    void iterate_cycle();
    task::Task* find_lowest_task_with_request();
    
  public:
    OptimisticManager(task::TaskTable task_table, task::ResourceTable resource_table)
    {
        cycle_ = 0;
        this->task_table = task_table;
        this->resource_table_ = resource_table;
    }
    void do_tasks();
    void print();
};
} // namespace manager