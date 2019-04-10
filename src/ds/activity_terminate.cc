#include "task.h"
#include "activity.h"
#include "activity_terminate.h"

namespace task
{

void Termination::execute() 
{
    // Activity::execute();
}

bool Termination::dispatch(Task *target_task, int cycle)
{
    return target_task->terminate(cycle);
}
} // namespace task