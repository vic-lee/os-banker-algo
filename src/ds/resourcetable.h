#ifndef HEADER_RESOURCETABLE
#define HEADER_RESOURCETABLE

#include <map>
#include <vector>
#include <string>
#include "resource.h"
#include "activity_request.h"
#include "activity_release.h"

namespace task
{

class ResourceTable
{
    std::vector<Resource *> resource_table_;
    std::vector<Release *> pending_release_table_;
    Resource* find_resource_by_id(int id);
    int get_resource_count(int resource_id);

  public:
    bool can_satisfy_request(Request *request);
    bool handle_new_request(Request *request);
    void reverse_request(Request *request);
    void handle_new_release(Release *release);
    void release_pending_resources();
    void add(Resource *resource);
    void print();
};

} // namespace task

#endif