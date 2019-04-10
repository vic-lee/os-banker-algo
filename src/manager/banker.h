#include "manager.h"

namespace manager
{

class Banker : public Manager
{
  public:
    Banker(task::TaskTable task_table, task::ResourceTable resource_table);
    virtual ~Banker();
    void do_tasks();

  private:
    void iterate_cycle();
    bool is_state_safe();
    void update_resources_available_vector(std::vector<int> &resources_available, std::vector<int> resources_released);

    void before_cycle_setup();
    void after_cycle_teardown();

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

    void incr_blocked_task_waiting_time();

    std::map<int, bool> visit_table_;

};
} // namespace manager