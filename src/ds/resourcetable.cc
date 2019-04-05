#include "resourcetable.h"

namespace task
{

int ResourceTable::get_resource_count(int resource_id)
{
    return resource_table[resource_id].get_unit_count();
}

bool ResourceTable::can_satisfy_request(Request request)
{
    int resource_type = request.get_resource_type();
    return resource_table[resource_type].can_satisfy_request(request);
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