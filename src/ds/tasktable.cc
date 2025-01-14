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

void TaskTable::add(Task task)
{
    task_table_.insert(std::pair<int, Task>(task.id(), task));
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
    task::Task *t = &task_table_.at(id);
    return t;
}

bool TaskTable::has_task_been_created(int id)
{
    return task_table_.count(id) > 0;
}

bool TaskTable::is_all_task_terminated()
{
    std::map<int, Task>::iterator it;

    for (it = task_table_.begin(); it != task_table_.end(); it++)
    {
        if (!it->second.is_terminated())
            return false;
    }

    return true;
}

int TaskTable::size()
{
    return task_table_.size();
}

void TaskTable::print()
{
    std::map<int, Task>::iterator it;

    for (it = task_table_.begin(); it != task_table_.end(); it++)
    {
        it->second.print();
    }
}

void TaskTable::add_new_activity_to_task(Activity *activity)
{
    int target_id = activity->get_target_id();
    task_table_.find(target_id)->second.add_new_activity(activity);
}

std::vector<std::vector<int>> TaskTable::generate_unmet_demand_matrix() 
{
    std::vector<std::vector<int>> unmet_demand;

    for (unsigned int i = 1; i < (task_table_.size() + 1); i++)
    {
        task::Task *task = access_task_by_id(i);
        std::vector<int> unmet_demand_for_task = task->generate_unmet_demand_vector();
        unmet_demand.push_back(unmet_demand_for_task);
    }

    return unmet_demand;
}

} // namespace task