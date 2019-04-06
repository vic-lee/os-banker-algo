#include <map>
#include <iostream>

#include "task.h"
#include "activity.h"
#include "tasktable.h"
#include "activity_request.h"
#include "activity_release.h"
#include "activity_terminate.h"

namespace task
{

int str_to_int(std::string s)
{
    return atoi(s.c_str());
}

void TaskTable::add(Task task)
{
    task_table.insert(std::pair<int, Task>(task.id(), task));
}

void TaskTable::create_task_from_input(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int claimed_resource_type = str_to_int(parsed_line[3]);
    int claimed_resource_count = str_to_int(parsed_line[4]);

    task::Claim claim(claimed_resource_type, claimed_resource_count);
    task::Task new_task(task_id, claim);

    this->add(new_task);
}

void TaskTable::add_new_request_to_task(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int delay = str_to_int(parsed_line[2]);
    int request_type = str_to_int(parsed_line[3]);
    int request_count = str_to_int(parsed_line[4]);
    task::Request new_request(task_id, delay, request_type, request_count);
    this->add_new_activity_to_task(new_request);
}

void TaskTable::add_new_release_to_task(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int delay = str_to_int(parsed_line[2]);
    int release_type = str_to_int(parsed_line[3]);
    int release_count = str_to_int(parsed_line[4]);
    task::Release new_release(task_id, delay, release_type, release_count);
    this->add_new_activity_to_task(new_release);
}

void TaskTable::add_termination_to_task(std::vector<std::string> parsed_line)
{
    int task_id = str_to_int(parsed_line[1]);
    int delay = str_to_int(parsed_line[2]);
    task::Termination new_termination(task_id, delay);
    this->add_new_activity_to_task(new_termination);
}

task::Task* TaskTable::access_task_by_id(int id)
{
    task::Task* t = &task_table.at(id);
    return t;
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

void TaskTable::add_new_activity_to_task(Activity activity)
{
    int target_id = activity.get_target_id();
    task_table.find(target_id)->second.add_new_activity(activity);
}

} // namespace task