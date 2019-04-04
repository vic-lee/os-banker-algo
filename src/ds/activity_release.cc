#include <iostream>
#include "activity_release.h"

namespace task
{

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