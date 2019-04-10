#include <iostream>
#include "task.h"
#include "activity.h"
#include "resourcetable.h"

namespace task
{

Activity::Activity(std::string type, int target_id, int delay)
{
    this->type_ = type;
    this->target_id = target_id;
    this->delay = delay;
    this->time_remaining_ = delay;
    this->completed_ = false;
}

void Activity::set_to_complete()
{
    completed_ = true;
}

int Activity::get_target_id()
{
    return target_id;
}

void Activity::update_time_remaining_before_execute()
{
    if (completed_)
    {
        return;
    }
    else if (has_begun_ && time_remaining_ == 0)
    {
        return;
    }
    else if (has_begun_ && time_remaining_ > 0)
    {
        time_remaining_--;
        return;
    }
    else
    {
        has_begun_ = true;
    }
}

void Activity::update_completion_state_after_execute(bool is_successful)
{
    /**
     * Only call this function after Activity is executed (as the 
     * function name indicates).
     * 
     * An activity may satisfy `has_begun_` and `time_remaining_==0`
     * immeidately after `time_remaining_` has been updated, but has
     * yet been executed. 
     */
    if (is_successful && has_begun_ && time_remaining_ == 0)
    {
        completed_ = true;
    }
}

bool Activity::is_active()
{
    return !completed_;
}

bool Activity::is_computing()
{
    return (has_begun_ && time_remaining_ > 0);
}

bool Activity::is_time_to_execute()
{
    return time_remaining_ == 0;
}

std::string Activity::type()
{
    return type_;
}

bool Activity::is_initiate()
{
    return type_ == "initiate";
}

bool Activity::is_request()
{
    return type_ == "request";
}

bool Activity::is_release()
{
    return type_ == "release";
}

bool Activity::is_termination()
{
    return type_ == "terminate";
}

void Activity::print()
{
    std::cout << type_ << ":\t"
              << " Target ID: " << target_id
              << " Delay: " << delay 
              << " Completed: " << completed_ << std::endl;
}

bool Activity::dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) 
{
    return false;
}

} // namespace task