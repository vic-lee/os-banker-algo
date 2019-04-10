#include <iostream>
#include "claim.h"
#include "resourcetable.h"

namespace task
{
bool Claim::is_claim_legal(ResourceTable *resource_table)
{
    int max_num_of_units = resource_table
                               ->find_resource_by_id(claimed_resource_id)
                               ->get_unit_count();

    bool is_legal = claim_count <= max_num_of_units;

    if (!is_legal)
    {
        std::cout << "Claim of " << claim_count
                  << " RT" << claimed_resource_id << " is not legal; "
                  << "Total unit count: " << max_num_of_units << std::endl;
    }

    return is_legal;
}

void Claim::print()
{
    std::cout << "Resource ID: " << claimed_resource_id
              << "\t\tClaim Count: " << claim_count << std::endl;
}
} // namespace task