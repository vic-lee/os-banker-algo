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
    int get_resource_count(int resource_id);

  public:
    bool can_satisfy_request(Request *request);
    bool can_satisfy_request_next_cycle(Request *request);
    bool handle_new_request(Request *request);
    void reverse_request(Request *request);
    void handle_new_release(Release *release);
    void release_pending_resources();

    std::vector<int> generate_resource_available_vector();

    Resource *find_resource_by_id(int id);
    void add(Resource *resource);
    void print();
};

} // namespace task

#endif