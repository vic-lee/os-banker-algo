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
}

int Resource::get_unit_count()
{
    return total_unit_count_;
}

bool Resource::can_satisfy_request(Request *request)
{
    int num_of_units_needed = request->get_request_count();
    return (remaining_unit_count_ > num_of_units_needed);
}

bool Resource::handle_new_request(Request *request)
{
    if (!can_satisfy_request(request))
    {
        int num_of_units_needed = request->get_request_count();
        int target_id = request->get_target_id();
        std::cout
            << "Cannot satisfy request; " 
            << "Task " << target_id << " requests " 
            << num_of_units_needed << " of RT " << id_ << "; "
            << "Only has " << remaining_unit_count_ <<" "
            << std::endl;
        return false;
    }
    else
    {
        int num_of_units_needed = request->get_request_count();
        remaining_unit_count_ -= num_of_units_needed;
        std::cout << "Request of " << num_of_units_needed << " of RT " << id_
                  << " granted to Task " << request->get_target_id() << "; "
                  << "Total: " << total_unit_count_ << "; "
                  << "Remaining: " << remaining_unit_count_ << std::endl;
        return true;
    }
}

void Resource::handle_new_release(Release *release)
{
    int num_of_units_restored = release->get_release_count();
    remaining_unit_count_ += num_of_units_restored;

    std::cout << "Release of " << num_of_units_restored << " of RT " << id_
              << " granted to Task " << release->get_target_id() << "; "
              << "Total: " << total_unit_count_ << "; "
              << "Remaining: " << remaining_unit_count_ << std::endl;
}

void Resource::print()
{
    std::cout << "Resource ID: " << id_
              << "\tUnit count: " << total_unit_count_
              << "\tRemaining count: " << remaining_unit_count_ << std::endl;
}
} // namespace task