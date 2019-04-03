#ifndef HEADER_ACTIVITY_RELEASE
#define HEADER_ACTIVITY_RELEASE

#include <string>
#include "activity.h"

namespace task
{

class Release : public Activity
{
    int release_resource_type; 
    int release_count;
  public:
    Release(std::string type, int target_id, int delay, int release_resource_type, int release_count) 
    : Activity(type, target_id, delay) {}
};

} // namespace task

#endif