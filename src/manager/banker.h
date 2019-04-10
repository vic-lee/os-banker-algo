#include "manager.h"

namespace manager
{

class Banker : public Manager
{
public:
    Banker(task::TaskTable task_table, task::ResourceTable resource_table, bool debug);
    virtual ~Banker();
    void do_tasks();

private:
    void iterate_cycle();
    bool is_state_safe();
    void update_resources_available_vector(std::vector<int> &resources_available, std::vector<int> resources_released);

    void do_latest_initiates();
    void do_latest_terminates();
    void do_latest_releases();
    void do_latest_requests();
    void do_latest_requests_from_blocked_tasks();
    void do_latest_requests_from_non_blocked_tasks();

    bool do_task_latest_activity(task::Task *t);

    bool should_visit_task(task::Task *t);

    void mark_as_visited(task::Task *t);
    bool has_visited(task::Task *t);
};
} // namespace manager