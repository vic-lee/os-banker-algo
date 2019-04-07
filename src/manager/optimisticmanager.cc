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
    std::cout << "\n----- Cycle: " << cycle << " -----" << std::endl;
    bool been_deadlocked = false;

    while (does_deadlock_exist())
    {
        std::cout << "DEADLOCKED!!!" << std::endl;
        been_deadlocked = true;
        handle_deadlock();
    }
    
    if (!been_deadlocked)
    {
        for (int i = 1; i < (task_table.size() + 1); i++)
        {
            task::Task *task = task_table.access_task_by_id(i);
            task->do_latest_activity(&resource_table_, cycle);
        }

        // task::Task *task;
        // int id = 1;

        // std::tie(task, id) = task_table.get_next_active_task(0);

        // while (id != -1)
        // {   
        //     task->do_latest_activity(&resource_table_, cycle);
        //     std::tie(task, id) = task_table.get_next_active_task(id);
        // }
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

        if (!task->is_terminated() && task->is_latest_activity_request())
        {
            has_request_pending_next_cycle = true;

            std::cout << "Testing Task " << task->id()
                      << "; Terminated? " << task->is_terminated()
                      << "; Aborted? " << task->is_aborted() << std::endl;

            task::Activity *latest_request = task->get_latest_activity();

            bool can_satisfy = resource_table_.can_satisfy_request(
                static_cast<task::Request *>(latest_request));
            if (can_satisfy)
            {
                does_deadlock_exist = false;
            }
            else
            {
                task->increment_cycles_waiting();
            }
            
        }
    }

    if (!has_request_pending_next_cycle && does_deadlock_exist == true)
        does_deadlock_exist = false;

    return does_deadlock_exist;
}

void OptimisticManager::handle_deadlock()
{
    task::Task *task_to_abort = find_lowest_task_with_request();

    if (task_to_abort != NULL)
    {
        task_to_abort->abort(&resource_table_);
    }
}

task::Task *OptimisticManager::find_lowest_task_with_request()
{
    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task *task = task_table.access_task_by_id(i);
        if (task->is_latest_activity_request())
            return task;
    }
    return nullptr;
}

void OptimisticManager::print()
{
    std::cout << "\n/******** FIFO ********/" <<std::endl;

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
              << (100 * cumulative_time_waiting / (double)cumulative_time_spent) << "%"
              << std::endl;
}

} // namespace manager