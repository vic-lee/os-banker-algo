#include <iostream>
#include <iomanip>
#include <math.h>
#include "manager.h"
#include "../ds/task.h"
#include "../ds/activity.h"
#include "../ds/activity_request.h"
#include "optimisticmanager.h"

namespace manager
{
OptimisticManager::OptimisticManager(
    task::TaskTable task_table,
    task::ResourceTable resource_table,
    bool debug) : Manager(task_table, resource_table, debug)
{
    this->should_check_safety_ = false;
}

void OptimisticManager::do_tasks()
{
    while (!task_table_.is_all_task_terminated())
        iterate_cycle();
}

void OptimisticManager::iterate_cycle()
{
    // std::cout << "\n----- Cycle " << cycle_ << "-" << cycle_ + 1 << " -----" << std::endl;

    std::map<int, bool> visit_status = create_visit_status_table_for_all_tasks();

    do_all_latest_releases(visit_status);
    do_all_latest_initiates(visit_status);
    do_all_latest_terminates(visit_status);

    while (does_deadlock_exist(visit_status))
    {
        std::cout << "DEADLOCKED!!!" << std::endl;
        handle_deadlock();
    }

    do_all_latest_requests(visit_status);

    resource_table_.release_pending_resources();

    cycle_++;
}

bool OptimisticManager::does_deadlock_exist(std::map<int, bool> visit_status)
{
    bool does_deadlock_exist = true;

    bool has_request_pending_this_cycle = false;

    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = task_table_.access_task_by_id(i);

        if (visit_status.at(i) == true)
        {
            does_deadlock_exist = false;
            continue;
        }

        if (!task->is_terminated() && task->is_latest_activity_request())
        {
            has_request_pending_this_cycle = true;

            task::Activity *latest_request = task->get_latest_activity();

            bool can_satisfy = resource_table_.can_satisfy_request_next_cycle(
                static_cast<task::Request *>(latest_request));

            if (can_satisfy)
                does_deadlock_exist = false;
        }
    }

    if (!has_request_pending_this_cycle && does_deadlock_exist == true)
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
    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = task_table_.access_task_by_id(i);
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

    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *current_task = task_table_.access_task_by_id(i);
        current_task->print_finished_status();

        int time_spent, time_waiting;
        std::tie(time_spent, time_waiting) = current_task->get_print_statistic();
        cumulative_time_spent += time_spent;
        cumulative_time_waiting += time_waiting;
    }
    std::cout << "Total\t    "
              << std::setw(4) << cumulative_time_spent
              << std::setw(4) << cumulative_time_waiting
              << std::setw(4) << (int)nearbyint(100 * cumulative_time_waiting / ((double)cumulative_time_spent)) << "%"
              << std::endl;
}

} // namespace manager