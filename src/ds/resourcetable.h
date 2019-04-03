#ifndef HEADER_RESOURCETABLE
#define HEADER_RESOURCETABLE

#include <vector>
#include <string>
#include "resource.h"

namespace task
{

class ResourceTable
{
    std::vector<Resource> resource_table;

  public:
    int get_resource_count(int resource_id);
    void add(Resource resource);
    void print();
};

} // namespace task

#endif