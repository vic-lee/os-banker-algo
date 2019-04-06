#include <iostream>
#include "task.h"
#include "claim.h"
#include "activity.h"
#include "activity_request.h"

namespace task
{

Task::Task(int id, Claim claim)
{
    this->id_ = id;
    terminated_ = false;
    claims_table_.insert(
        std::pair<int, Claim>(claim.claimed_resource_id, claim));
}

void Task::set_latest_activity()
{
    for (int i = 0; i < activities_table_.size(); i++)
    {
        if (activities_table_[i].is_active())
            latest_activity_index_ = i;
    }
    latest_activity_index_ = -1;
}

void Task::add_new_activity(Activity activity)
{
    activities_table_.push_back(activity);
}

Activity &Task::get_latest_activity()
{
    set_latest_activity();
    return activities_table_[latest_activity_index_];
}

void Task::do_latest_activity(ResourceTable &resource_table)
{
    set_latest_activity();

    if (latest_activity_index_ == -1)
        return;

    Activity latest_activity = get_latest_activity();

    latest_activity.update_time_remaining_before_execute();

    if (latest_activity.is_time_to_execute())
    {
        execute_activity(latest_activity, resource_table);
        latest_activity.update_completion_state_after_execute();
    }
}

void Task::execute_activity(Activity &latest_activity, ResourceTable &resource_table)
{
    if (latest_activity.is_request())
        resource_table.handle_new_request(static_cast<Request *>(&latest_activity));

    else if (latest_activity.is_release())
        resource_table.handle_new_release(static_cast<Release *>(&latest_activity));

    else if (latest_activity.is_termination())
        terminate();
}

int Task::id()
{
    return id_;
}

bool Task::is_terminated()
{
    return terminated_;
}

void Task::terminate()
{
    terminated_ = true;
}

void Task::print()
{
    std::cout << std::endl;

    std::cout << "Task ID: " << id_ << std::endl;

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