#include <iostream>
#include "activity_request.h"

namespace task
{

Request::Request(int target_id, int delay, int request_resource_type, int request_count)
    : Activity("request", target_id, delay)
{
    this->request_resource_type_ = request_resource_type;
    this->request_count_ = request_count;
    std::cout << "Request RT: " << this->request_resource_type_
              << "\tRequest count: " << this->request_count_ << std::endl;
}

int Request::get_resource_type()
{
    std::cout << request_resource_type_ << std::endl;
    return request_resource_type_;
}

int Request::get_request_count()
{
    return request_count_;
}

void Request::execute()
{
    // Activity::execute();
}

} // namespace task