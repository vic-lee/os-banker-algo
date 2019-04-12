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
    /**
     * Algorithm:
     * First, the optimistic manager performs releases, initiates, and terminates.
     * Second, it checks if a deadlock exist; if exists, it handles by 
     * removing the lowest numbered task until a deadlock no longer exists.
     * Third, it does requests. 
     * 
     * The setup and teardown functions are identical to that for the Banker's algorithm. 
     */

    while (!task_table_.is_all_task_terminated())
        iterate_cycle();
}

void OptimisticManager::iterate_cycle()
{
    before_cycle_setup();

    do_latest_releases();
    do_latest_initiates();
    do_latest_terminates();

    while (does_deadlock_exist())   /* Tasks are aborted until a deadlock no longer exist. */
    {
        // std::cout << "Deadlocked!" << std::endl;
        handle_deadlock();
    }

    do_latest_requests();           /* Perform requests only after deadlocks are removed. */

    after_cycle_teardown();

    cycle_++;
}

bool OptimisticManager::does_deadlock_exist()
{
    /**
     * This function checks if a deadlock exist in the system. Assuming a 
     * deadlock exists in the system, the algorithm tries to prove otherwise. 
     * 
     * There are a few scenarios where a deadlock does not exist. 
     * First, if a task has performed an activity, a deadlock does not exist. 
     * 
     * Second, if there exists pending requests in this cycle, and that request
     * can be satisfied next cycle (we consider next cycle, not this cycle, since
     * resources may have been released before checking deadlock), then a 
     * deadlock does not exist. 
     * 
     * Third, if there is no pending request, there is no deadlock. 
     * 
     * If any of the three conditions above are proven to be true, a deadlock
     * does not exist and the function returns false. Otherwise it returns true. 
     */

    bool does_deadlock_exist = true;

    bool has_request_pending_this_cycle = false;

    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = task_table_.access_task_by_id(i);

        if (visit_table_.at(i) == true)
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
    /**
     * This function aborts lowest active task (and thereby releasing that 
     * task's resources) if a deadlock exists.
     */

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

void OptimisticManager::do_latest_requests()
{
    do_latest_requests_from_blocked_tasks();
    do_latest_requests_from_non_blocked_tasks();
}

void OptimisticManager::do_latest_requests_from_blocked_tasks()
{
    unsigned int i = 0;
    while (i < blocked_tasks_table_.size())
    {
        task::Task *task = blocked_tasks_table_[i];
        bool is_task_unblocked = false;

        is_task_unblocked = do_one_latest_request(task, true);

        if (is_task_unblocked)
            i--;
        i++;
    }
}

void OptimisticManager::do_latest_requests_from_non_blocked_tasks()
{
    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = task_table_.access_task_by_id(i);
        do_one_latest_request(task, false);
    }
}

bool OptimisticManager::do_one_latest_request(task::Task *task, bool from_blocked)
{
    /**
     * This function handles one request in the FIFO algorithm. 
     * 
     * If a task's latest request failed, and this is not because 
     * 1) it is computing, 2) it is already in a blocked state, 3) the task 
     * was aborted, then this task is added to the blocked table.
     * 
     * Likewise, if a request is satisfied and the task is currently in the 
     * blocked table, it is removed from the blocked table. 
     */

    int id = task->id();

    bool should_do_task = !task->is_terminated() && visit_table_.at(id) == false;

    if (!from_blocked)
        should_do_task = !is_in_blocked_table(id) && should_do_task;

    if (should_do_task)
    {
        task::Activity *activity = task->get_latest_activity();

        if (activity->is_request())
        {
            bool is_successful = task->do_latest_activity(&resource_table_, cycle_, should_check_safety_);
            visit_table_.at(id) = true;

            if (!is_successful && !task->is_computing() && !task->is_aborted() && !from_blocked)
            {
                blocked_tasks_table_.push_back(task);
            }

            else if (is_successful && from_blocked)
            {
                remove_from_blocked_table(task);
                return true;
            }
        }
    }
    return false;
}

} // namespace manager