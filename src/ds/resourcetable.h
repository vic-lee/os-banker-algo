#ifndef HEADER_RESOURCETABLE
#define HEADER_RESOURCETABLE

#include <vector>
#include <string>
#include "resource.h"
#include "activity_request.h"

namespace task
{

class ResourceTable
{
    std::vector<Resource> resource_table;

    int get_resource_count(int resource_id);

  public:
    bool can_satisfy_request(Request *request);
    void handle_new_request(Request *request);
    void add(Resource resource);
    void print();
};

} // namespace task

#endif