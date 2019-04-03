#ifndef HEADER_ACTIVITY_REQUEST
#define HEADER_ACTIVITY_REQUEST

#include <string>
#include "activity.h"

namespace task
{

class Request : public Activity
{
    int request_resource_type; 
    int request_count;
  public:
    Request(int target_id, int delay, int request_resource_type, int request_count) 
    : Activity("request", target_id, delay) 
    {
        this->request_resource_type = request_resource_type;
        this->request_count = request_count;
    }
};

} // namespace task

#endif