#include "manager.h"

namespace manager
{
Manager::Manager(task::TaskTable task_table, task::ResourceTable resource_table)
{
    cycle_ = 0;
    this->task_table_ = task_table;
    this->resource_table_ = resource_table;
}
Manager::~Manager() {}
void Manager::do_tasks() {}
void Manager::print() {}
} // namespace manager