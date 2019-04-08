#ifndef HEADER_MANAGER
#define HEADER_MANAGER

#include <vector>
#include "../ds/task.h"
#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace manager
{

class Manager
{
  public:
    Manager(task::TaskTable task_table, task::ResourceTable resource_table);
    virtual ~Manager();
    virtual void do_tasks();
    virtual void print();

  protected:
    int cycle_;
    task::TaskTable task_table_;
    std::vector<task::Task *> blocked_tasks_table_;
    task::ResourceTable resource_table_;
};
} // namespace manager

#endif