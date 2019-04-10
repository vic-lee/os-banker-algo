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

private:
    bool does_deadlock_exist();
    void handle_deadlock();
    void iterate_cycle();
    task::Task *find_lowest_task_with_request();

    void do_all_latest_requests();
    void do_latest_requests_from_blocked_tasks();
    void do_latest_requests_from_non_blocked_tasks();
    bool do_one_latest_request(task::Task *task, bool from_blocked);
};
} // namespace manager