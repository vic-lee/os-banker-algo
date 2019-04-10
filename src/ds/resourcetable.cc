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
    int resource_type = release->get_resource_type();
    int release_count = release->get_release_count();
    Resource *target_resource = find_resource_by_id(resource_type);
    target_resource->add_release_next_cycle(release_count);
}

void ResourceTable::release_pending_resources()
{
    for (auto& resource : resource_table_)
    {
        resource->clear_to_be_added_units();
    }
}

std::vector<int> ResourceTable::generate_resource_available_vector()
{
    std::vector<int> resource_available;

    for (auto& resource : resource_table_)
    {
        int availability = resource->remaining_unit_count();
        resource_available.push_back(availability);
    }
    
    return resource_available;
}

void ResourceTable::add(Resource *resource)
{
    resource_table_.push_back(resource);
}

void ResourceTable::print()
{
    for (auto& resource : resource_table_)
        resource->print();
}
} // namespace task