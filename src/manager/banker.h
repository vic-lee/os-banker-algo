#include "manager.h"

namespace manager
{

class Banker : public Manager
{

    /**
     * The Banker class extends the Manager base class. 
     * It encapsulates functionalities specific to the Banker's algorithm. 
     */

public:
    Banker(task::TaskTable task_table, task::ResourceTable resource_table, bool debug);
    virtual ~Banker();
    void do_tasks() override;

private:
    void iterate_cycle() override;

    void do_latest_requests() override;
    void do_latest_requests_from_blocked_tasks();
    void do_latest_requests_from_non_blocked_tasks();

    bool is_request_safe(task::Task *task);
};
} // namespace manager