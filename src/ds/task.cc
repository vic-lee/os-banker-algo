#include <iostream>
#include "task.h"
#include "activity.h"
#include "activity_request.h"

namespace task
{


void Task::set_latest_activity()
{
    for (int i = 0; i < activities_table_.size(); i++)
    {
        if (activities_table_[i].is_active())
            latest_activity_index_ = i;
    }
    latest_activity_index_ = -1;
}

void Task::do_request(Request& request, ResourceTable& resource_table)
{    

}

void Task::add_new_activity(Activity activity)
{
    activities_table_.push_back(activity);
}

Activity& Task::get_latest_activity()
{
    set_latest_activity();
    return activities_table_[latest_activity_index_];
}

void Task::do_latest_activity(ResourceTable& resource_table)
{
    set_latest_activity();
    
    if (latest_activity_index_ == -1)
        return;

    
    activities_table_[latest_activity_index_].execute();
}

int Task::get_id()
{
    return id;
}

bool Task::is_terminated()
{
    return terminated;
}

void Task::print()
{
    std::cout << std::endl;

    std::cout << "Task ID: " << id << std::endl;

    std::cout << "---------------- Claims ---------------" << std::endl;
    std::map<int, Claim>::iterator it;
    for (it = claims_table_.begin(); it != claims_table_.end(); it++)
    {
        it->second.print();
    }

    std::cout << "------------- Activities --------------" << std::endl;
    for (int i = 0; i < activities_table_.size(); i++)
    {
        activities_table_[i].print();
    }
}

} // namespace task