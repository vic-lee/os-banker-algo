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

    if (!(remaining_unit_count_ >= num_of_units_needed))
    {
        std::cout << "Task " << request->get_target_id()
                  << "'s request of " << num_of_units_needed << " RT"
                  << request->get_resource_type() << " cannot be granted; "
                  << remaining_unit_count_ << " units remaining" << std::endl;
    }

    return (remaining_unit_count_ >= num_of_units_needed);
}

bool Resource::can_satisfy_request_next_cycle(Request *request)
{
    int num_of_units_needed = request->get_request_count();
    bool can_satisfy = (remaining_unit_count_ + units_to_be_added_next_cycle_) >= num_of_units_needed;

    if (!can_satisfy)
    {
        std::cout << "Task " << request->get_target_id()
                  << "'s request of " << num_of_units_needed << " RT"
                  << request->get_resource_type()
                  << " cannot be granted (next cycle); "
                  << remaining_unit_count_ << " units remaining" << std::endl;
    }

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

        // std::cout << "Task " << request->get_target_id()
        //           << " completes its request of "
        //           << num_of_units_needed << " of RT " << id_ << "; "
        //           << "Total: " << total_unit_count_ << "; "
        //           << "Remaining: " << remaining_unit_count_ << std::endl;

        return true;
    }
}

void Resource::reverse_request(Request *request)
{
    int num_of_units_to_restore = request->get_request_count();
    remaining_unit_count_ += num_of_units_to_restore;
    // std::cout << "Restored " << num_of_units_to_restore << " of RT " << id_
    //           << " originally given to Task " << request->get_target_id() << "; "
    //           << "Total: " << total_unit_count_ << "; "
    //           << "Remaining: " << remaining_unit_count_ << std::endl;
}

void Resource::reverse_request(int unit_count)
{
    remaining_unit_count_ += unit_count;
}

void Resource::handle_new_release(Release *release)
{
    int num_of_units_restored = release->get_release_count();
    remaining_unit_count_ += num_of_units_restored;

    /** DEBUG **/
    // if (remaining_unit_count_ > total_unit_count_)
    //     std::cout << "BUG: units remaining > total units; "
    //               << "Remaining " << remaining_unit_count_
    //               << "; Total " << total_unit_count_ << std::endl;

    // std::cout << "Release of " << num_of_units_restored << " of RT " << id_
    //           << " granted to Task " << release->get_target_id() << "; "
    //           << "Total: " << total_unit_count_ << "; "
    //           << "Remaining: " << remaining_unit_count_ << std::endl;
}

void Resource::add_release_next_cycle(int unit_count)
{
    units_to_be_added_next_cycle_ += unit_count;
}

void Resource::clear_to_be_added_units()
{
    if (units_to_be_added_next_cycle_ > 0)
    {
        // std::cout << "Release " << units_to_be_added_next_cycle_
        //           << " units of RT" << id_ << std::endl;
    }

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