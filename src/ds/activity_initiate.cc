#include "task.h"
#include "claim.h"
#include "resourcetable.h"
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

bool Initiate::dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal)
{
    if (check_legal)
    {
        bool is_claim_legal = claim_->is_claim_legal(resource_table);
        if (!is_claim_legal)
        {
            target_task->abort(resource_table);
            return false;
        }
    }
    target_task->add_new_claim(claim_);
    std::cout << "Initiating Task " << target_task->id() << std::endl;
    return true;
}
} // namespace task