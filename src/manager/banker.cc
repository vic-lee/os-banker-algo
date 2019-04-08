#include "banker.h"

namespace manager
{
Banker::Banker(task::TaskTable task_table, task::ResourceTable resource_table)
    : Manager(task_table, resource_table) {}
} // namespace managero