#include <iostream>
#include "activity.h"

namespace task
{

void Activity::set_to_complete()
{
    completed = true;
}

int Activity::get_target_id()
{
    return target_id;
}

void Activity::do_activity() {}

void Activity::print()
{
    std::cout << type << ":\t"
              << " Target ID: " << target_id
              << " Delay: " << delay << std::endl;
}

} // namespace task