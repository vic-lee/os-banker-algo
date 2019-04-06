#include <iostream>
#include "resourcetable.h"

namespace task
{

Resource* ResourceTable::find_resource_by_id(int id)
{
    return resource_table_[id - 1];
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
    Resource* target_resource = find_resource_by_id(resource_type);
    target_resource->handle_new_request(request);
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
    for (int i = 0; i < resource_table_.size(); i++)
    {
        resource_table_[i]->print();
    }
}

} // namespace task