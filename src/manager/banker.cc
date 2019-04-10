#include <vector>
#include "banker.h"
#include "../ds/task.h"
#include "../ds/tasktable.h"

namespace manager
{
Banker::Banker(task::TaskTable task_table, task::ResourceTable resource_table)
    : Manager(task_table, resource_table)
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
    // do_latest_requests()    // abort if request > total unit
    // {
    //     do_blocked_tasks_requests();
    //     do_regular_tasks_requests();
    // }
    // do_initiates();     // abort if claim > total unit
    // do_terminates();
    // do_releases();
    std::map<int, bool> visit_status = create_visit_status_table_for_all_tasks();

    before_cycle_setup();

    do_latest_requests();
    do_latest_releases();
    do_latest_initiates();
    do_latest_terminates();

    // do_all_latest_initiates(visit_status);
    // do_all_latest_terminates(visit_status);
    // do_all_latest_releases(visit_status);

    after_cycle_teardown();

    cycle_++;
}

void Banker::print() {}

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
}

void Banker::after_cycle_teardown()
{
    visit_table_.clear();
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
            bool did_successfully_request = blocked_task->do_latest_activity(&resource_table_, cycle_);

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
            bool was_activity_successful = task->do_latest_activity(&resource_table_, cycle_);

            if (!was_activity_successful)
                block(task);
        }
    }
}

bool Banker::should_visit_task(task::Task *t)
{
    /**
     *  Technically has_visit is redundant here; because the algorithm 
     * only visits blocked tasks before visiting non-blocked tasks. 
     * However, for safety's sake, it is checked one more time here. 
     */
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

void Banker::update_resources_available_vector(
    std::vector<int> &resources_available,
    std::vector<int> resources_released)
{
}

} // namespace manager