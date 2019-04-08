#ifndef HEADER_ACTIVITY_TERMINATION
#define HEADER_ACTIVITY_TERMINATION

#include <string>
#include "activity.h"

namespace task
{

class Termination : public Activity
{
  public:
    Termination(int target_id, int delay, Task *task) 
    : Activity("terminate", target_id, delay, task) {}
    void execute();
};

} // namespace task

#endif