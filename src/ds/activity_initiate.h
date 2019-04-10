#ifndef HEADER_ACTIVITY_INITIATE
#define HEADER_ACTIVITY_INITIATE

#include <string>
#include "activity.h"
#include "claim.h"

namespace task
{

class Initiate : public Activity
{
  public:
    Initiate(int target_id, Claim claim, Task *task);
    Claim * claim();

  private:
    Claim claim_;
};

} // namespace task

#endif