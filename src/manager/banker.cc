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
     * Requests:
     *      First, check if it is safe to satisfy blocked tasks' requests.
     *          If safe, satisfy & remove from blocked. 
     *      Second, check if it is safe to satisfy regular tasks' requests.
     *          If safe, satisfy
     *          If not safe, add to blocked list and incr. wait time. 
     * Initiate. 
     * Terminate. 
     * Release. 
     */

    while (!task_table_.is_all_task_terminated())
    {
        iterate_cycle();
    }
}

void Banker::iterate_cycle()
{
    std::cout << "\n------ Cycle " << cycle_ << "-" << (cycle_ + 1) << " ------" << std::endl;

    before_cycle_setup();

    do_latest_requests();
    do_latest_releases();
    do_latest_initiates();
    do_latest_terminates();

    after_cycle_teardown();

    cycle_++;
}

bool Banker::is_state_safe()
{
    bool is_state_safe = false;

    std::vector<std::vector<int>> unmet_demand = task_table_.generate_unmet_demand_matrix();
    std::vector<int> resource_available = resource_table_.generate_resource_available_vector();

    return is_state_safe;
}

void Banker::before_cycle_setup()
{
    if (visit_table_.size() > 0)
        visit_table_.clear();

    visit_table_ = create_visit_status_table_for_all_tasks();

    decr_delay_countdowns();
}

void Banker::after_cycle_teardown()
{
    resource_table_.release_pending_resources();

    visit_table_.clear();

    incr_blocked_task_waiting_time();
}

void Banker::do_latest_requests()
{
    do_latest_requests_from_blocked_tasks();
    do_latest_requests_from_non_blocked_tasks();
}

void Banker::do_latest_requests_from_blocked_tasks()
{
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

void Banker::do_latest_releases()
{
    for (int i = 0; i < task_table_.size(); i++)
    {
        int id = i + 1;
        task::Task *task = task_table_.access_task_by_id(id);

        if (should_visit_task(task) && task->is_latest_activity_release())
        {
            do_task_latest_activity(task);
        }
    }
}

void Banker::do_latest_initiates()
{
    for (int i = 0; i < task_table_.size(); i++)
    {
        int id = i + 1;
        task::Task *task = task_table_.access_task_by_id(id);

        if (should_visit_task(task) && task->is_latest_activity_initiate())
        {
            do_task_latest_activity(task);
        }
    }
}

void Banker::do_latest_terminates()
{
    for (int i = 0; i < task_table_.size(); i++)
    {
        int id = i + 1;
        task::Task *task = task_table_.access_task_by_id(id);

        if (should_visit_task(task) && task->is_latest_activity_terminate())
        {
            do_task_latest_activity(task);
        }
    }
}

bool Banker::do_task_latest_activity(task::Task *t)
{
    bool was_successful = t->do_latest_activity(&resource_table_, cycle_, should_check_safety_);
    mark_as_visited(t);
    return was_successful;
}

bool Banker::should_visit_task(task::Task *t)
{
    return (!is_in_blocked_table(t->id()) && !has_visited(t) && t->is_active());
}

void Banker::mark_as_visited(task::Task *t)
{
    visit_table_.at(t->id()) = true;
}

bool Banker::has_visited(task::Task *t)
{
    return visit_table_.at(t->id());
}

void Banker::incr_blocked_task_waiting_time()
{
    for (auto &blocked_task : blocked_tasks_table_)
    {
        blocked_task->increment_cycles_waiting(cycle_);
    }
}

void Banker::decr_delay_countdowns()
{
    for (int i = 0; i < task_table_.size(); i++)
    {
        int id = i + 1;
        task_table_.access_task_by_id(id)->do_latest_activity_delay_countdown();
    }
}

void Banker::update_resources_available_vector(
    std::vector<int> &resources_available,
    std::vector<int> resources_released)
{
}

} // namespace manager