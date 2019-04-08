#include "manager.h"

namespace manager
{

class Banker : public Manager
{
  public:
    Banker(task::TaskTable task_table, task::ResourceTable resource_table);
    void do_tasks();
    void print();

  private:
    bool is_state_safe();
};
} // namespace manager