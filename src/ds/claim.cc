#include <iostream>
#include "claim.h"

namespace task
{

void Claim::print()
{
    std::cout << "Resource ID: " << claimed_resource_id
              << "\t\tClaim Count: " << claim_count << std::endl;
}
} // namespace task