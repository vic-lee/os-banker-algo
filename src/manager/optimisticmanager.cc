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

void OptimisticManager::iterate_cycle(int &cycle)
{
    std::cout << "Cycle: " << cycle << std::endl;

    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task *task = task_table.access_task_by_id(i);
        task->do_latest_activity(&resource_table, cycle);
    }

    cycle++;
}

void OptimisticManager::print()
{
    int cumulative_time_spent = 0;
    int cumulative_time_waiting = 0;

    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task *current_task = task_table.access_task_by_id(i);
        current_task->print_finished_status();

        int time_spent, time_waiting;
        std::tie(time_spent, time_waiting) = current_task->get_print_statistic();
        cumulative_time_spent += time_spent;
        cumulative_time_waiting += time_waiting;
    }
    std::cout << "Total\t    "
              << cumulative_time_spent << "   "
              << cumulative_time_waiting << "   "
              << (cumulative_time_waiting / (double)cumulative_time_spent) << "%"
              << std::endl;
}

} // namespace manager