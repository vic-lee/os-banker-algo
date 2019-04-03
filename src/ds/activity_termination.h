#ifndef HEADER_ACTIVITY_TERMINATION
#define HEADER_ACTIVITY_TERMINATION

#include <string>
#include "activity.h"

namespace task
{

class Termination : public Activity
{
  public:
    Termination(std::string type, int delay) : Activity(type, delay) {}
};

} // namespace task

#endif