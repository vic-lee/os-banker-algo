#include <iostream>
#include "manager.h"

namespace manager
{
Manager::Manager(task::TaskTable task_table, task::ResourceTable resource_table)
{
    cycle_ = 0;
    this->task_table_ = task_table;
    this->resource_table_ = resource_table;
}

Manager::~Manager() {}

void Manager::do_tasks() {}

void Manager::print() {}

void Manager::remove_from_blocked_table(task::Task *t)
{
    for (int i = 0; i < blocked_tasks_table_.size(); i++)
    {
        if (blocked_tasks_table_[i]->id() == t->id())
            blocked_tasks_table_.erase(blocked_tasks_table_.begin() + i);
    }
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

void Manager::do_all_latest_requests(std::map<int, bool> &visit_status)
{
    do_all_blocked_tasks_with_requests(visit_status);
    do_all_non_blocked_tasks_with_requests(visit_status);
}

void Manager::do_all_blocked_tasks_with_requests(std::map<int, bool> &visit_status)
{
    int i = 0;
    while (i < blocked_tasks_table_.size())
    {
        task::Task *task = blocked_tasks_table_[i];
        bool is_task_unblocked = do_one_latest_activity_of_type("request", visit_status, task, true);
        if (is_task_unblocked)
            i--;
        i++;
    }
}

void Manager::do_all_non_blocked_tasks_with_requests(std::map<int, bool> &visit_status)
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
            bool is_successful = task->do_latest_activity(&resource_table_, cycle_);
            visit_status.at(id) = true;

            if (!is_successful && !task->is_computing() && !from_blocked)
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