#include <iostream>
#include "activity_release.h"

namespace task
{

void Release::execute() 
{
    Activity::execute();
}

int Release::get_resource_type()
{
    return release_resource_type;
}

int Release::get_release_count()
{
    return release_count;
}

void Release::print()
{
    std::cout << type << ":\t"
              << " Target ID: " << target_id
              << " Delay: " << delay 
              << " RT: " << release_resource_type
              << " RC: " << release_count
              << std::endl;
}
} // namespace task