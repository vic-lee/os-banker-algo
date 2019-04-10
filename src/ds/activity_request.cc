#include <iostream>
#include "task.h"
#include "activity_request.h"

namespace task
{
Request::Request(int target_id, int delay, int request_resource_type, int request_count)
    : Activity("request", target_id, delay)
{
    this->request_resource_type_ = request_resource_type;
    this->request_count_ = request_count;
}

bool Request::dispatch(ResourceTable *resource_table, bool check_legal)
{
    if ((check_legal && is_request_legal()) || !check_legal)
    {
        bool is_successful = resource_table->handle_new_request(this);
        return is_successful;
    }
    return false;
}

bool Request::is_request_legal()
{
    int max_addl_demand = target_task_->check_unmet_demand_for_resource(request_resource_type_);

    if (request_count_ > max_addl_demand)
    {
        std::cout << "Task " << target_task_->id() << "'s request of RT"
                  << request_resource_type_ << " is not legal;"
                  << "Requested " << request_count_
                  << "; MaxAddlDemand: " << max_addl_demand << std::endl;
        return false;
    }
    return true;
}

int Request::get_resource_type()
{
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

void Request::print()
{
    std::cout << type_ << ":\t"
              << " Target ID: " << target_id
              << " Delay: " << delay
              << " Request RT: " << this->request_resource_type_
              << " Request count: " << this->request_count_ << std::endl;
}

} // namespace task