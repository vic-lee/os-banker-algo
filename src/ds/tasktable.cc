#include <map>
#include <iostream>

#include "task.h"
#include "activity.h"
#include "tasktable.h"
#include "activity_request.h"
#include "activity_release.h"
#include "activity_initiate.h"
#include "activity_terminate.h"

namespace task
{

int str_to_int(std::string s)
{
    return atoi(s.c_str());
}

std::tuple<Task *, int> TaskTable::get_next_active_task(int prior_id)
{
    int id = prior_id++;
    std::cout << "Doing ID " << id << std::endl;
    while (id <= task_table.size())
    {
        Task *task = access_task_by_id(id);
        if (!task->is_terminated())
            return std::make_tuple(task, id);
        id++;
    }

    return std::make_tuple(nullptr, -1);
}

void TaskTable::iterate_task_activities(bool been_deadlocked, ResourceTable *resource_table, int cycle)
{
    std::map<int, bool> visit_status = create_visit_status_table_for_all_tasks();

    do_all_latest_initiates(visit_status, resource_table, cycle);
    do_all_latest_terminates(visit_status, resource_table, cycle);

    if (!been_deadlocked)
        do_all_latest_requests(visit_status, resource_table, cycle);
        
    do_all_latest_releases(visit_status, resource_table, cycle);
}

std::map<int, bool> TaskTable::create_visit_status_table_for_all_tasks()
{
    std::map<int, bool> visit_table;

    for (int i = 1; i < (task_table.size() + 1); i++)
        visit_table.insert(std::pair<int, bool>(i, false));

    return visit_table;
}

void TaskTable::do_all_latest_initiates(
    std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle)
{
    do_all_latest_activity_of_type("initiate", visit_status, resource_table, cycle);
}

void TaskTable::do_all_latest_terminates(
    std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle)
{
    do_all_latest_activity_of_type("terminate", visit_status, resource_table, cycle);
}

void TaskTable::do_all_latest_requests(
    std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle)
{
    do_all_latest_activity_of_type("request", visit_status, resource_table, cycle);
}

void TaskTable::do_all_latest_releases(
    std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle)
{
    do_all_latest_activity_of_type("release", visit_status, resource_table, cycle);
}

void TaskTable::do_all_latest_activity_of_type(
    std::string type, std::map<int, bool> &visit_status, ResourceTable *resource_table, int cycle)
{
    for (int i = 1; i < (task_table.size() + 1); i++)
    {
        Task *task = access_task_by_id(i);

        if (!task->is_terminated() && visit_status.at(i) == false)
        {
            Activity *activity = task->get_latest_activity();

            if (activity->type() == type)
            {
                task->do_latest_activity(resource_table, cycle);
                visit_status.at(i) = true;
            }
        }
    }
}

void TaskTable::add(Task task)
{
    task_table.insert(std::pair<int, Task>(task.id(), task));
}

void TaskTable::handle_new_initiate(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);

    if (!this->has_task_been_created(task_id))
    {
        task::Task new_task(task_id);
        this->add(new_task);
    }
    int claimed_resource_type = str_to_int(parsed_line[3]);
    int claimed_resource_count = str_to_int(parsed_line[4]);

    task::Claim claim(claimed_resource_type, claimed_resource_count);
    Initiate *initiate = new Initiate(task_id, claim);
    this->add_new_activity_to_task(initiate);
}

void TaskTable::add_new_request_to_task(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int delay = str_to_int(parsed_line[2]);
    int request_type = str_to_int(parsed_line[3]);
    int request_count = str_to_int(parsed_line[4]);
    task::Request *request = new Request(task_id, delay, request_type, request_count);
    this->add_new_activity_to_task(request);
}

void TaskTable::add_new_release_to_task(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int delay = str_to_int(parsed_line[2]);
    int release_type = str_to_int(parsed_line[3]);
    int release_count = str_to_int(parsed_line[4]);
    task::Release *release = new Release(task_id, delay, release_type, release_count);
    this->add_new_activity_to_task(release);
}

void TaskTable::add_termination_to_task(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int delay = str_to_int(parsed_line[2]);
    task::Termination *termination = new Termination(task_id, delay);
    this->add_new_activity_to_task(termination);
}

task::Task *TaskTable::access_task_by_id(int id)
{
    task::Task *t = &task_table.at(id);
    return t;
}

bool TaskTable::has_task_been_created(int id)
{
    return task_table.count(id) > 0;
}

bool TaskTable::is_all_task_terminated()
{
    std::map<int, Task>::iterator it;

    for (it = task_table.begin(); it != task_table.end(); it++)
    {
        if (!it->second.is_terminated())
            return false;
    }

    return true;
}

int TaskTable::size()
{
    return task_table.size();
}

void TaskTable::print()
{
    std::map<int, Task>::iterator it;

    for (it = task_table.begin(); it != task_table.end(); it++)
    {
        it->second.print();
    }
}

void TaskTable::add_new_activity_to_task(Activity *activity)
{
    int target_id = activity->get_target_id();
    task_table.find(target_id)->second.add_new_activity(activity);
}

} // namespace task