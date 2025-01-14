#include <iostream>
#include "task.h"
#include "resourcetable.h"
#include "activity_release.h"

namespace task
{
Release::Release(int target_id, int delay, int release_resource_type, int release_count)
    : Activity("release", target_id, delay)
{
    this->release_resource_type = release_resource_type;
    this->release_count = release_count;
}

bool Release::dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle)
{
    /**
     * This function releases resources. 
     * 
     * Releasing a resource is composed of two actions: 
     * 
     * 1. Adding the number of units restored back to the resource table (Available next cycle)
     * 
     * 2. Removing the number of units from the Task's resource ownership table. 
     */

    resource_table->handle_new_release(this);

    target_task->release_resource_owned(this);

    return true;
}

int Release::get_resource_type()
{
    return release_resource_type;
}

int Release::get_release_count()
{
    return release_count;
}

void Release::print()
{
    std::cout << type_ << ":\t"
              << " Target ID: " << target_id
              << " Delay: " << delay
              << " Release RT: " << release_resource_type
              << " Release Count: " << release_count
              << std::endl;
}
} // namespace task