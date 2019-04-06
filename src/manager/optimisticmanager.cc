#include <iostream>
#include "../ds/task.h"
#include "optimisticmanager.h"

namespace manager
{

void OptimisticManager::do_tasks()
{
    int cycle = 0;

    while (!task_table.is_all_task_terminated() && cycle < 5)
        iterate_cycle(cycle);

}

void OptimisticManager::iterate_cycle(int& cycle)
{
    std::cout << "Cycle: " << cycle << std::endl;
    
    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task* task = task_table.access_task_by_id(i);
        task->do_latest_activity(&resource_table, cycle);
    }

    cycle++;
}

void OptimisticManager::print()
{

}

}