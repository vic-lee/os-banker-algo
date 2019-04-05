#include "resourcetable.h"

namespace task
{

int ResourceTable::get_resource_count(int resource_id)
{
    return resource_table[resource_id].get_unit_count();
}

bool ResourceTable::can_request_be_satisfied(Request request)
{
    int resource_type = request.get_resource_type();
    int request_count = request.get_request_count();
    int num_of_resource_available = get_resource_count(resource_type);
    return (num_of_resource_available > request_count);
}


void ResourceTable::add(Resource resource)
{
    resource_table.push_back(resource);
}

void ResourceTable::print()
{
    for (int i = 0; i < resource_table.size(); i++)
    {
        resource_table[i].print();
    }
}

} // namespace task