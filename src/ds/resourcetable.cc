#include "resourcetable.h"

namespace task
{

int ResourceTable::get_resource_count(int resource_id)
{
    return resource_table[resource_id].get_unit_count();
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