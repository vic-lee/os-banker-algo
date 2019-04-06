#include <iostream>
#include "resourcetable.h"

namespace task
{

Resource* ResourceTable::access_by_id(int id)
{
    return resource_table_[id];
}

int ResourceTable::get_resource_count(int resource_id)
{
    return resource_table_[resource_id]->get_unit_count();
}

bool ResourceTable::can_satisfy_request(Request *request)
{
    int resource_type = request->get_resource_type();
    return resource_table_[resource_type]->can_satisfy_request(request);
}

void ResourceTable::handle_new_request(Request *request)
{
    int resource_type = request->get_resource_type();
    std::cout << "Resource type is " << resource_type << std::endl;
    resource_table_[resource_type]->print();
    resource_table_[resource_type]->handle_new_request(request);
}

void ResourceTable::handle_new_release(Release *release)
{
    int resource_type = release->get_resource_type();
    resource_table_[resource_type]->handle_new_release(release);
}

void ResourceTable::add(Resource *resource)
{
    resource_table_.push_back(resource);
}

void ResourceTable::print()
{
    for (int i = 0; i < resource_table.size(); i++)
    {
        resource_table[i]->print();
    }
}

} // namespace task