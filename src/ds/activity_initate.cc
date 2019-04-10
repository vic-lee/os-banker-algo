#include "claim.h"
#include "activity_initiate.h"

namespace task
{
Initiate::Initiate(int target_id, Claim claim, Task *task)
    : Activity("initiate", target_id, 0, task)
{
    this->claim_ = claim;
}

Claim *Initiate::claim()
{
    return &claim_;
}
} // namespace task