#ifndef HEADER_TASK
#define HEADER_TASK

#include <string>
#include <map>
#include <vector>

#include "resource.h"
#include "activity.h"
#include "claim.h"
#include "resourcetable.h"

namespace task
{

typedef int resource_id, claim_count;

class Task
{
    int id;
    int latest_activity_;
    bool terminated;
    std::map<int, Claim> claims_table;
    std::vector<Activity> activities_table;

    void set_latest_activity();
    void do_request(Request& request, ResourceTable& resource_table);

  public:
    Task(int id, Claim claim)
    {
        this->id = id;
        terminated = false;
        claims_table.insert(
            std::pair<int, Claim>(claim.claimed_resource_id, claim));
    }
    void add_new_activity(Activity activity);
    void do_latest_activity(ResourceTable& resource_table);
    int get_id();
    std::string to_string();
    bool is_terminated();
    void print();
};
} // namespace task

#endif