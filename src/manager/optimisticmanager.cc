#include <iostream>
#include "../ds/task.h"
#include "../ds/activity.h"
#include "../ds/activity_request.h"
#include "optimisticmanager.h"

namespace manager
{

void OptimisticManager::do_tasks()
{
    while (!task_table.is_all_task_terminated())
        iterate_cycle();
}

void OptimisticManager::iterate_cycle()
{
    std::cout << "\n----- Cycle " << cycle_ << "-" << cycle_ + 1 << " -----" << std::endl;

    bool been_deadlocked = false;

    std::map<int, bool> visit_status = task_table.create_visit_status_table_for_all_tasks();

    task_table.do_all_latest_releases(visit_status, &resource_table_, cycle_);

    while (does_deadlock_exist(visit_status))
    {
        std::cout << "DEADLOCKED!!!" << std::endl;
        been_deadlocked = true;
        handle_deadlock();
    }

    task_table.do_all_latest_initiates(visit_status, &resource_table_, cycle_);
    task_table.do_all_latest_terminates(visit_status, &resource_table_, cycle_);

    if (!been_deadlocked)
        task_table.do_all_latest_requests(visit_status, &resource_table_, cycle_);

    resource_table_.release_pending_resources();

    cycle_++;
}

bool OptimisticManager::does_deadlock_exist(std::map<int, bool> visit_status)
{
    bool does_deadlock_exist = true;

    bool has_request_pending_next_cycle = false;

    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        task::Task *task = task_table.access_task_by_id(i);

        if (visit_status.at(i) == true)
        {
            does_deadlock_exist = false;
            continue;
        }

        if (!task->is_terminated() && task->is_latest_activity_request())
        {
            has_request_pending_next_cycle = true;

            task::Activity *latest_request = task->get_latest_activity();

            bool can_satisfy = resource_table_.can_satisfy_request(
                static_cast<task::Request *>(latest_request));
            if (can_satisfy)
            {
                does_deadlock_exist = false;
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
    std::cout << "\n/******** FIFO ********/" << std::endl;

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