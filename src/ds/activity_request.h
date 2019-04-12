#ifndef HEADER_ACTIVITY_REQUEST
#define HEADER_ACTIVITY_REQUEST

#include <string>
#include "activity.h"

namespace task
{
class Task;
class ResourceTable;

class Request : public Activity
{

    /**
     * The Request class is a subclass of the Activity class. 
     * 
     * A request is defined if and only if resource request type and number
     * of units to request are specified. 
     * 
     * The dispatch function, overriden here, is responsible for carrying 
     * out the Request. Critically, a request may not be granted: either 
     * the request may exceed the task's claim (the Banker's algorithm) or the request
     * cannot be satisfied by the request table (number request > number availble).
     */

public:
    Request(int target_id, int delay, int request_resource_type, int request_count);
    int get_resource_type();
    int get_request_count();
    bool is_request_legal(Task *target_task);
    void print() override;

    bool dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) override;

private:
    int request_resource_type_;
    int request_count_;
};

} // namespace task

#endif