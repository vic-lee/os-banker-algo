#include "task.h"
#include "activity.h"
#include "resourcetable.h"
#include "activity_terminate.h"

namespace task
{
bool Termination::dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle)
{
    return target_task->terminate(cycle);
}
} // namespace task