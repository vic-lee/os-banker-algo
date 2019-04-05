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
    Release(int target_id, int delay, int release_resource_type, int release_count) 
    : Activity("release", target_id, delay) 
    {
        this->release_resource_type = release_resource_type;
        this->release_count = release_count;
    }
    void print();
    void do_optimistic();
};

} // namespace task

#endif