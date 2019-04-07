#include <iostream>
#include "../ds/task.h"
#include "../ds/activity.h"
#include "../ds/activity_request.h"
#include "optimisticmanager.h"

namespace manager
{

void OptimisticManager::do_tasks()
{
    int cycle = 0;

    while (!task_table.is_all_task_terminated())
        iterate_cycle(cycle);
}

void OptimisticManager::iterate_cycle(int &cycle)
{
    std::cout << "Cycle: " << cycle << std::endl;

    if (does_deadlock_exist())
    {
        std::cout << "DEADLOCKED!!!" << std::endl;
        handle_deadlock();
    }

    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task *task = task_table.access_task_by_id(i);
        task->do_latest_activity(&resource_table_, cycle);
    }

    cycle++;
}

bool OptimisticManager::does_deadlock_exist()
{
    bool does_deadlock_exist = true;

    bool has_request_pending_next_cycle = false;

    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task *task = task_table.access_task_by_id(i);

        if (task->is_latest_activity_request())
        {
            has_request_pending_next_cycle = true;

            task::Activity *latest_request = task->get_latest_activity();

            bool can_satisfy = resource_table_.can_satisfy_request(
                static_cast<task::Request *>(latest_request));
            if (can_satisfy)
            {
                does_deadlock_exist = false;
                std::cout << does_deadlock_exist << std::endl;
            }
        }
    }

    if (!has_request_pending_next_cycle && does_deadlock_exist==true)
        does_deadlock_exist = false;

    return does_deadlock_exist;
}

void OptimisticManager::handle_deadlock()
{
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