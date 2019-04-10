#ifndef HEADER_ACTIVITY_REQUEST
#define HEADER_ACTIVITY_REQUEST

#include <string>
#include "activity.h"

namespace task
{
class Task;
class ResourceTable;

class Request : public Activity
{
  public:
    Request(int target_id, int delay, int request_resource_type, int request_count);
    int get_resource_type();
    int get_request_count();
    bool is_request_legal(Task *target_task);
    void execute();
    void print() override;

    bool dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) override;

  private:
    int request_resource_type_;
    int request_count_;
};

} // namespace task

#endif