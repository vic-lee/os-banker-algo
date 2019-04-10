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
    Initiate(int target_id, Claim claim);
    bool dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) override;
    Claim *claim();

private:
    Claim claim_;
};

} // namespace task

#endif