#ifndef HEADER_ACTIVITY_RELEASE
#define HEADER_ACTIVITY_RELEASE

#include <string>
#include "activity.h"

namespace task
{
class ResourceTable;

class Release : public Activity
{
  public:
    Release(int target_id, int delay, int release_resource_type, int release_count);
    void print();
    void execute();
    int get_resource_type();
    int get_release_count();

    bool dispatch(ResourceTable *resource_table);

  private:
    int release_resource_type;
    int release_count;
};

} // namespace task

#endif