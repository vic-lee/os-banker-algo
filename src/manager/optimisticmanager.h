#include "manager.h"
#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace manager
{

class OptimisticManager : public Manager
{
  public:
    OptimisticManager(task::TaskTable task_table, task::ResourceTable resource_table, bool debug);
    void do_tasks();
    void print();

  private:
    bool does_deadlock_exist(std::map<int, bool> visit_status);
    void handle_deadlock();
    void iterate_cycle();
    task::Task *find_lowest_task_with_request();
};
} // namespace manager