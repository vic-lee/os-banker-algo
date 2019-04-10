#include "claim.h"
#include "activity_initiate.h"

namespace task
{
Initiate::Initiate(int target_id, Claim claim)
    : Activity("initiate", target_id, 0)
{
    this->claim_ = claim;
}

Claim *Initiate::claim()
{
    return &claim_;
}
} // namespace task