#ifndef HEADER_ACTIVITY_REQUEST
#define HEADER_ACTIVITY_REQUEST

#include <string>

namespace task
{

class Activity;
class ResourceTable;

class Request : public Activity
{
  public:
    Request(int target_id, int delay, int request_resource_type, int request_count, Task *task);
    int get_resource_type();
    int get_request_count();
    void execute();
    void print() override;

    bool dispatch(ResourceTable *resource_table, bool check_legal);

  private:
    bool is_request_legal();
    int request_resource_type_;
    int request_count_;
};

} // namespace task

#endif