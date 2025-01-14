#ifndef HEADER_CLAIM
#define HEADER_CLAIM

namespace task
{

class ResourceTable;

class Claim
{
    /**
     * This is a simple data structure storing the two elements that define a claim:
     * what resource is the claim on, and how many units does it claim. 
     */

public:
    int claimed_resource_id;
    int claim_count;
    Claim(){};
    Claim(int claimed_resource_id, int claim_count)
    {
        this->claimed_resource_id = claimed_resource_id;
        this->claim_count = claim_count;
    }

    bool is_claim_legal(ResourceTable *resource_table);

    void print();
};
} // namespace task

#endif