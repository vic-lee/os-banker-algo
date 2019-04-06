#include <iostream>
#include "activity.h"

namespace task
{

void Activity::set_to_complete()
{
    completed = true;
}

int Activity::get_target_id()
{
    return target_id;
}

void Activity::execute()
{
    if (completed)
    {
        return;
    }
    else if (executed && time_remaining_ == 0)
    {
        completed = true;
        return;
    }
    else if (executed && time_remaining_ > 0)
    {
        time_remaining_--;
        return;
    }
    else
    {
        executed = true;
    }
}

bool Activity::is_active()
{
    return !completed;
}

bool Activity::is_time_to_execute()
{
    return time_remaining_ == 0;
}

std::string Activity::type()
{
    return type_;
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
              << " Delay: " << delay << std::endl;
}

} // namespace task