#include <iostream>
#include "activity.h"

namespace task
{

int Activity::get_target_id()
{
    return target_id;
}

void Activity::print()
{
    std::cout << type << ":\t"
              << "Target ID: " << target_id
              << "Delay: " << delay << std::endl;
}

} // namespace task