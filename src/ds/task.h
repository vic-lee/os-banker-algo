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
    int latest_activity_index_;
    bool terminated_;
    std::map<int, Claim> claims_table_;
    std::vector<Activity> activities_table_;

    void set_latest_activity();
    Activity& get_latest_activity();
    void execute_activity(Activity& latest_activity, ResourceTable& resource_table);

  public:
    Task(int id, Claim claim)
    {
        this->id = id;
        terminated_ = false;
        claims_table_.insert(
            std::pair<int, Claim>(claim.claimed_resource_id, claim));
    }
    void add_new_activity(Activity activity);
    void do_latest_activity(ResourceTable& resource_table);
    int get_id();
    std::string to_string();
    bool is_terminated();
    void terminate();
    void print();
};
} // namespace task

#endif