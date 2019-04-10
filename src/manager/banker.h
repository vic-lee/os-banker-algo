#include "manager.h"

namespace manager
{

class Banker : public Manager
{
public:
    Banker(task::TaskTable task_table, task::ResourceTable resource_table, bool debug);
    virtual ~Banker();
    void do_tasks() override;

private:
    void iterate_cycle();
    bool is_state_safe();

    void do_latest_requests() override;
    void do_latest_requests_from_blocked_tasks();
    void do_latest_requests_from_non_blocked_tasks();
};
} // namespace manager