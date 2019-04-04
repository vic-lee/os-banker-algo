#include <iostream>
#include "resource.h"

namespace task
{

int Resource::get_unit_count()
{
    return unit_count;
}

void Resource::print()
{
    std::cout << "Resource ID:\t" << id
              << "\tUnit count:\t" << unit_count << std::endl;
}
} // namespace task