#include <iostream>
#include "resource.h"
#include "activity_request.h"

namespace task
{

Resource::Resource(int id, int unit_count)
{
    this->id_ = id;
    this->total_unit_count_ = unit_count;
    this->remaining_unit_count_ = unit_count;
    this->units_to_be_added_next_cycle_ = 0;
}

int Resource::get_unit_count()
{
    return total_unit_count_;
}

int Resource::remaining_unit_count()
{
    return remaining_unit_count_;
}

bool Resource::can_satisfy_request(Request *request)
{
    int num_of_units_needed = request->get_request_count();

    return (remaining_unit_count_ >= num_of_units_needed);
}

bool Resource::can_satisfy_request_next_cycle(Request *request)
{
    int num_of_units_needed = request->get_request_count();
    bool can_satisfy = (remaining_unit_count_ + units_to_be_added_next_cycle_) >= num_of_units_needed;

    return can_satisfy;
}

bool Resource::handle_new_request(Request *request)
{
    if (!can_satisfy_request(request))
    {
        return false;
    }
    else
    {
        int num_of_units_needed = request->get_request_count();
        remaining_unit_count_ -= num_of_units_needed;

        return true;
    }
}

void Resource::reverse_request(Request *request)
{
    int num_of_units_to_restore = request->get_request_count();
    remaining_unit_count_ += num_of_units_to_restore;
}

void Resource::reverse_request(int unit_count)
{
    remaining_unit_count_ += unit_count;
}

void Resource::handle_new_release(Release *release)
{
    int num_of_units_restored = release->get_release_count();
    remaining_unit_count_ += num_of_units_restored;
}

void Resource::add_release_next_cycle(int unit_count)
{
    units_to_be_added_next_cycle_ += unit_count;
}

void Resource::clear_to_be_added_units()
{
    remaining_unit_count_ += units_to_be_added_next_cycle_;
    units_to_be_added_next_cycle_ = 0;
}

void Resource::print()
{
    std::cout << "Resource ID: " << id_
              << "\tUnit count: " << total_unit_count_
              << "\tRemaining count: " << remaining_unit_count_ << std::endl;
}
} // namespace task