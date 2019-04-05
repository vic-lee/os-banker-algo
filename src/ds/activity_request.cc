#include "activity_request.h"

namespace task
{

int Request::get_resource_type()
{
    return request_resource_type;
}

int Request::get_request_count()
{
    return request_count;
}

void Request::execute()
{
    if (completed)
    {
        return;
    }
    else if (executed && time_remaining == 0)
    {
        completed = true;
        return;
    }
    else if (executed && time_remaining > 0)
    {
        time_remaining--;
        return;
    }
    else
    {
        executed = true;
    }
}

} // namespace task