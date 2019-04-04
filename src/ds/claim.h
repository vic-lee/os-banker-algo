#ifndef HEADER_CLAIM
#define HEADER_CLAIM

namespace task
{

class Claim
{
  public:
    int claimed_resource_id;
    int claim_count;

    Claim(int claimed_resource_id, int claim_count)
    {
        this->claimed_resource_id = claimed_resource_id;
        this->claim_count = claim_count;
    }

    void print();
};
} // namespace task

#endif