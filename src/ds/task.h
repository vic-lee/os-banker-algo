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
    int latest_activity;
    bool terminated;
    std::map<int, Claim> claims_table;
    std::vector<Activity> activities_table;

    int set_latest_activity();

  public:
    Task(int id, Claim claim)
    {
        this->id = id;
        terminated = false;
        claims_table.insert(
            std::pair<int, Claim>(claim.claimed_resource_id, claim));
    }
    void add_new_activity(Activity activity);
    int get_id();
    std::string to_string();
    bool is_terminated();
    void print();
};
} // namespace task

#endif