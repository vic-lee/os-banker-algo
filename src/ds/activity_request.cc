#include <iostream>
#include "task.h"
#include "activity_request.h"
#include "../manager/manager.h"

namespace task
{
Request::Request(int target_id, int delay, int request_resource_type, int request_count)
    : Activity("request", target_id, delay)
{
    this->request_resource_type_ = request_resource_type;
    this->request_count_ = request_count;
}

bool Request::dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle)
{
    /**
     * This function executes the Request activity.
     *  
     * Check legal is true when the callee is the Banker's algorithm, since 
     * 1) the concept of claims exist for the Banker's algorithm, and 2) 
     * requests cannot go beyond a task's claim. 
     * 
     * Once dispatched, the task of granting the request is delegated first to
     * the ResourceTable, then (eventually) to a specific Resource itself. It 
     * is a Resource's responsibility to check if an incoming request exceeds 
     * the maximum availability, and decide whether to grant the request or not. 
     * 
     * Crucially, check_legal is different from checking if this request is safe.
     * it is not a Request's responsibility to determine whether a request is 
     * safe; the callee (the banker) is responsible for doing so. Dispatch should
     * only be called once the algorithm determines the call is safe.  
     */

    if (check_legal && !is_request_legal(target_task))
    {
        target_task->abort(resource_table);
        return false;
    }

    bool was_successful = resource_table->handle_new_request(this);

    if (was_successful)
        target_task->add_resource_owned(this);
    else
        target_task->increment_cycles_waiting(cycle);

    return was_successful;
}

bool Request::is_request_legal(Task *target_task)
{
    int max_addl_demand = target_task->check_unmet_demand_for_resource(request_resource_type_);

    if (request_count_ > max_addl_demand)
    {
        std::cout << "Task " << target_task->id() << "'s request of RT"
                  << request_resource_type_ << " is not legal; "
                  << "Requested " << request_count_
                  << "; Max addl demand: " << max_addl_demand << std::endl;
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

void Request::print()
{
    std::cout << type_ << ":\t"
              << " Target ID: " << target_id
              << " Delay: " << delay
              << " Request RT: " << this->request_resource_type_
              << " Request count: " << this->request_count_ << std::endl;
}

} // namespace task