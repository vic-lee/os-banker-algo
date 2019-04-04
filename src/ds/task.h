#ifndef HEADER_TASK
#define HEADER_TASK

#include <string>
#include <map>
#include <vector>

#include "resource.h"
#include "activity.h"
#include "claim.h"

namespace task
{

typedef int resource_id, claim_count;

class Task
{
    int id;
    std::map<int, int> claims_table;
    std::vector<Activity> activities_table;

  public:
    Task(int id, Claim claim)
    {
        this->id = id;
        claims_table.insert(
            std::pair<int, int>(claim.claimed_resource_id, claim.claim_count));
    }
    void add_new_activity(Activity activity);
    int get_id();
    std::string to_string();
};
} // namespace task

#endif