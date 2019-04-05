#include <iostream>
#include "resource.h"
#include "activity_request.h"

namespace task
{

int Resource::get_unit_count()
{
    return total_unit_count;
}

bool Resource::can_satisfy_request(Request request)
{
    int resource_type = request.get_resource_type();
    int num_of_units_needed = request.get_request_count();
    return (remaining_unit_count > num_of_units_needed);
}

void Resource::print()
{
    std::cout << "Resource ID:\t" << id
              << "\tUnit count:\t" << total_unit_count << std::endl;
}
} // namespace task