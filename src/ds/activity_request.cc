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
    // Activity::execute();
}

} // namespace task