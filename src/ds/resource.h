#ifndef HEADER_RESOURCE
#define HEADER_RESOURCE

#include "activity_request.h"
#include "activity_release.h"

namespace task
{

class Resource
{
  public:
    Resource(int id, int unit_count);
    int get_unit_count();
    bool can_satisfy_request(Request *request);
    void handle_new_request(Request *request);
    void handle_new_release(Release *release);
    void print();

  private:
    int id_;
    int total_unit_count_;
    int remaining_unit_count_;
};

} // namespace task

#endif