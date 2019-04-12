#include <iostream>
#include <iomanip>
#include <math.h>
#include "manager.h"

namespace manager
{
Manager::Manager(task::TaskTable task_table, task::ResourceTable resource_table, bool debug)
{
    cycle_ = 0;
    this->task_table_ = task_table;
    this->resource_table_ = resource_table;
    this->debug_ = debug;
}

Manager::~Manager() {}

void Manager::do_latest_releases()
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

void Manager::do_latest_initiates()
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

void Manager::do_latest_terminates()
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

void Manager::do_latest_requests() {}

bool Manager::do_task_latest_activity(task::Task *t)
{
    /**
     * This function groups doing activity and marking the visit status into 
     * an atomic action. No task should be called to perform an activity 
     * without marking as visited. 
     */

    bool was_successful = t->do_latest_activity(&resource_table_, cycle_, should_check_safety_);
    mark_as_visited(t);
    return was_successful;
}

bool Manager::should_visit_task(task::Task *t)
{
    /**
     * We should only perform actions on tasks if they are 
     * 1) not in the blocked table (they are visited before regular tasks are considered)
     * 2) not been visited in the cycle (multiple functions may iterate the entire task table)
     * 3) the task is active (i.e. not terminated or aborted)
     */

    return (!is_in_blocked_table(t->id()) && !has_visited(t) && t->is_active());
}

void Manager::block(task::Task *t)
{
    blocked_tasks_table_.push_back(t);
}

bool Manager::is_in_blocked_table(int id)
{
    for (auto &blocked_task : blocked_tasks_table_)
    {
        if (blocked_task->id() == id)
            return true;
    }
    return false;
}

void Manager::remove_from_blocked_table(task::Task *t)
{
    for (unsigned int i = 0; i < blocked_tasks_table_.size(); i++)
    {
        if (blocked_tasks_table_[i]->id() == t->id())
            blocked_tasks_table_.erase(blocked_tasks_table_.begin() + i);
    }
}

std::map<int, bool> Manager::create_visit_status_table_for_all_tasks()
{
    /**
     * This function creates a visit table for all tasks. 
     * 
     * The table is a mapping between task_id and a boolean describing 
     * whether the task has been visited. 
     */

    std::map<int, bool> visit_table;

    for (int i = 1; i < (task_table_.size() + 1); i++)
        visit_table.insert(std::pair<int, bool>(i, false));

    return visit_table;
}

bool Manager::has_visited(task::Task *t)
{
    return visit_table_.at(t->id());
}

void Manager::mark_as_visited(task::Task *t)
{
    visit_table_.at(t->id()) = true;
}

void Manager::before_cycle_setup()
{
    if (visit_table_.size() > 0)
        visit_table_.clear();

    visit_table_ = create_visit_status_table_for_all_tasks();

    decr_delay_countdowns();
}

void Manager::after_cycle_teardown()
{
    resource_table_.release_pending_resources();

    visit_table_.clear();

    incr_blocked_task_waiting_time();
}

void Manager::incr_blocked_task_waiting_time()
{
    for (auto &blocked_task : blocked_tasks_table_)
    {
        blocked_task->increment_cycles_waiting(cycle_);
    }
}

void Manager::decr_delay_countdowns()
{
    for (int i = 0; i < task_table_.size(); i++)
    {
        int id = i + 1;
        task_table_.access_task_by_id(id)->do_latest_activity_delay_countdown();
    }
}

void Manager::iterate_cycle() {}

void Manager::do_tasks() {}

void Manager::print()
{
    if (should_check_safety_ == true)
        std::cout << "\n/******** BANKER ********/" << std::endl;
    else
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
              << std::setw(4) << (int)nearbyint(100 * cumulative_time_waiting / ((double)cumulative_time_spent)) << "%\n"
              << std::endl;
}

} // namespace manager