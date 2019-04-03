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
    Request(std::string type, int target_id, int delay, int request_resource_type, int request_count) 
    : Activity(type, target_id, delay) {}
};

} // namespace task

#endif