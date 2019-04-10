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
              << std::setw(4) << (int)nearbyint(100 * cumulative_time_waiting / ((double)cumulative_time_spent)) << "%"
              << std::endl;

}

void Manager::remove_from_blocked_table(task::Task *t)
{
    for (int i = 0; i < blocked_tasks_table_.size(); i++)
    {
        if (blocked_tasks_table_[i]->id() == t->id())
            blocked_tasks_table_.erase(blocked_tasks_table_.begin() + i);
    }
}

void Manager::block(task::Task *t)
{
    // std::cout << "Blocking Task " << t->id() << std::endl;
    blocked_tasks_table_.push_back(t);
}

bool Manager::is_in_blocked_table(int id)
{
    for (int i = 0; i < blocked_tasks_table_.size(); i++)
    {
        if (blocked_tasks_table_[i]->id() == id)
            return true;
    }
    return false;
}

std::map<int, bool> Manager::create_visit_status_table_for_all_tasks()
{
    std::map<int, bool> visit_table;

    for (int i = 1; i < (task_table_.size() + 1); i++)
        visit_table.insert(std::pair<int, bool>(i, false));

    return visit_table;
}

bool Manager::is_request_safe(task::Task *task)
{
    bool is_request_safe = true;
    std::vector<int> max_addl_demand = task->generate_unmet_demand_vector();
    std::vector<int> current_resource_availability = resource_table_.generate_resource_available_vector();

    /* DEBUG */
    if (max_addl_demand.size() != current_resource_availability.size())
        std::cout << "Max demand and Availability vector has different sizes." << std::endl;

    for (int i = 0; i < max_addl_demand.size(); i++)
    {
        if (max_addl_demand[i] > current_resource_availability[i])
        {
            is_request_safe = false;
            // std::cout << "Request from Task " << task->id() << " is not safe; "
            //           << "MaxAddlDemand for RT" << i << " is " << max_addl_demand[i] << "; "
            //           << "Availability: " << current_resource_availability[i] << std::endl;
        }
    }

    return is_request_safe;
}

void Manager::do_all_latest_requests(std::map<int, bool> &visit_status)
{
    do_latest_requests_from_blocked_tasks(visit_status);
    do_latest_requests_from_non_blocked_tasks(visit_status);
}

void Manager::do_latest_requests_from_blocked_tasks(std::map<int, bool> &visit_status)
{
    int i = 0;
    while (i < blocked_tasks_table_.size())
    {
        task::Task *task = blocked_tasks_table_[i];
        bool is_task_unblocked = false;

        if (should_check_safety_) /*  BANKER  */
        {
            if (is_request_safe(task))
            {
                is_task_unblocked = do_one_latest_activity_of_type("request", visit_status, task, true);
            }
        }
        else /*  FIFO  */
        {
            is_task_unblocked = do_one_latest_activity_of_type("request", visit_status, task, true);
        }

        if (is_task_unblocked)
            i--;
        i++;
    }
}

void Manager::do_latest_requests_from_non_blocked_tasks(std::map<int, bool> &visit_status)
{
    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = task_table_.access_task_by_id(i);
        do_one_latest_activity_of_type("request", visit_status, task, false);
    }
}

void Manager::do_all_latest_initiates(std::map<int, bool> &visit_status)
{
    do_all_latest_activity_of_type("initiate", visit_status);
}

void Manager::do_all_latest_terminates(std::map<int, bool> &visit_status)
{
    do_all_latest_activity_of_type("terminate", visit_status);
}

void Manager::do_all_latest_releases(std::map<int, bool> &visit_status)
{
    do_all_latest_activity_of_type("release", visit_status);
}

void Manager::do_all_latest_activity_of_type(std::string type, std::map<int, bool> &visit_status)
{
    for (int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = task_table_.access_task_by_id(i);
        do_one_latest_activity_of_type(type, visit_status, task, false);
    }
}

bool Manager::do_one_latest_activity_of_type(
    std::string type, std::map<int, bool> &visit_status, task::Task *task, bool from_blocked)
{
    int id = task->id();

    bool should_do_task = !task->is_terminated() && visit_status.at(id) == false;

    if (!from_blocked)
        should_do_task = !is_in_blocked_table(id) && should_do_task;

    if (should_do_task)
    {
        task::Activity *activity = task->get_latest_activity();

        if (activity->type() == type)
        {
            bool is_successful = task->do_latest_activity(&resource_table_, cycle_, should_check_safety_);
            visit_status.at(id) = true;

            if (!is_successful && !task->is_computing() && !task->is_aborted() && !from_blocked)
            {
                std::cout << "Adding Task " << task->id() << " to blocked table." << std::endl;
                blocked_tasks_table_.push_back(task);
            }

            else if (is_successful && from_blocked)
            {
                std::cout << "Removing Task " << task->id() << " from blocked table." << std::endl;
                remove_from_blocked_table(task);
                return true;
            }
        }
    }
    return false;
}

} // namespace manager