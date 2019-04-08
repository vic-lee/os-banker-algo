#include <iostream>
#include "resourcetable.h"

namespace task
{

Resource *ResourceTable::find_resource_by_id(int id)
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
    return find_resource_by_id(resource_type)->can_satisfy_request(request);
}

bool ResourceTable::can_satisfy_request_next_cycle(Request *request)
{
    int resource_type = request->get_resource_type();
    return find_resource_by_id(resource_type)->can_satisfy_request_next_cycle(request);
}

bool ResourceTable::handle_new_request(Request *request)
{
    int resource_type = request->get_resource_type();
    Resource *target_resource = find_resource_by_id(resource_type);
    bool is_successful = target_resource->handle_new_request(request);
    return is_successful;
}

void ResourceTable::reverse_request(Request *request)
{
    int resource_type = request->get_resource_type();
    Resource *target_resource = find_resource_by_id(resource_type);
    target_resource->reverse_request(request);
}

void ResourceTable::handle_new_release(Release *release)
{
    // int resource_type = release->get_resource_type();
    // int num_of_units_restored = release->get_release_count();
    // Resource *target_resource = find_resource_by_id(resource_type);

    pending_release_table_.push_back(release);

    // target_resource->handle_new_release(release);
}

void ResourceTable::release_pending_resources()
{
    for (int i = 0; i < pending_release_table_.size(); i++)
    {
        Release *release = pending_release_table_[i];
        int resource_type = release->get_resource_type();
        Resource *target_resource = find_resource_by_id(resource_type);
        target_resource->handle_new_release(release);
    }
    pending_release_table_.clear();

    for (int i = 0; i < resource_table_.size(); i++)
    {
        resource_table_[i]->clear_to_be_added_units();
    }
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