#include <iostream>
#include <vector>
#include "banker.h"
#include "../ds/task.h"
#include "../ds/tasktable.h"

namespace manager
{
Banker::Banker(task::TaskTable task_table, task::ResourceTable resource_table, bool debug)
    : Manager(task_table, resource_table, debug)
{
    this->should_check_safety_ = true;
}

Banker::~Banker() {}

void Banker::do_tasks()
{
    /**
     * Algorithm:
     * 
     * Requests:
     *      First, check if it is safe to satisfy blocked tasks' requests.
     *          If safe, satisfy & remove from blocked. 
     *      Second, check if it is safe to satisfy non-blocked tasks' requests.
     *          If safe, satisfy
     *          If not safe, add to blocked list and incr. wait time. 
     * Initiate. 
     * Terminate. 
     * Release. 
     * 
     * Notes: 
     * - Each task is visited only once in a cycle. A visit table is set up at 
     * the beginning of a cycle to track each task's visit status. The visit 
     * table is reset at the end of a cycle. 
     * 
     * - Resources to be released are also released at the end of a cycle. 
     */

    while (!task_table_.is_all_task_terminated())
    {
        iterate_cycle();
    }
}

void Banker::iterate_cycle()
{
    before_cycle_setup(); /* Decr. delays */

    do_latest_requests();
    do_latest_releases();
    do_latest_initiates();
    do_latest_terminates();

    after_cycle_teardown(); /* Reset visit table; Release resources; Incr. waiting time */

    cycle_++;
}

void Banker::do_latest_requests()
{
    do_latest_requests_from_blocked_tasks();
    do_latest_requests_from_non_blocked_tasks();
}

void Banker::do_latest_requests_from_blocked_tasks()
{
    /**
     * This function considers if blocked tasks' requests can be fulfilled. 
     * If the request is safe and resources are indeed successfully assigned,
     * the task will be removed from the blocked table.
     */

    for (auto &blocked_task : blocked_tasks_table_)
    {
        if (is_request_safe(blocked_task))
        {
            bool did_successfully_request = do_task_latest_activity(blocked_task);

            if (did_successfully_request)
                remove_from_blocked_table(blocked_task);
        }
    }
}

void Banker::do_latest_requests_from_non_blocked_tasks()
{
    /**
     * This function considers if non-blocked tasks' requests can be fulfilled. 
     * If a request is safe, the request is granted; otherwise, the task is 
     * blocked and added to the blocked table. 
     * 
     * All tasks are in the task table (i.e. blocked table is a subset of tasks
     * in the task table). Therefore we should only visit tasks in the task 
     * table but *not* in the blocked table.
     */

    for (int i = 0; i < task_table_.size(); i++)
    {
        int id = i + 1;
        task::Task *task = task_table_.access_task_by_id(id);

        if (should_visit_task(task))
        {
            if (task->is_latest_activity_request())
            {
                if (is_request_safe(task))
                {
                    do_task_latest_activity(task);
                }
                else
                {
                    if (!task->is_computing())
                        block(task);
                }
            }
        }
    }
}

bool Banker::is_request_safe(task::Task *task)
{
    bool is_request_safe = true;
    std::vector<int> max_addl_demand = task->generate_unmet_demand_vector();
    std::vector<int> current_resource_availability = resource_table_.generate_resource_available_vector();

    for (unsigned int i = 0; i < max_addl_demand.size(); i++)
    {
        if (max_addl_demand[i] > current_resource_availability[i])
        {
            is_request_safe = false;
        }
    }

    return is_request_safe;
}

} // namespace manager