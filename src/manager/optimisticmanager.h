#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace manager
{

class OptimisticManager
{
    task::TaskTable task_table;
    task::ResourceTable resource_table;

    void iterate_cycle(int& cycle);

  public:
    OptimisticManager(task::TaskTable task_table, task::ResourceTable resource_table)
    {
        this->task_table = task_table;
        this->resource_table = resource_table;
    }
    void do_tasks();
    void print();
};
} // namespace manager