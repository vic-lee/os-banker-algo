#ifndef HEADER_TASK
#define HEADER_TASK

#include <string>
#include <map>
#include <vector>

#include "resource.h"
#include "activity.h"
#include "claim.h"
#include "resourcetable.h"
#include "activity_initiate.h"

namespace task
{

typedef int resource_id, claim_count;

class Task
{

    /**
     * This class encapsulates data and actions related to a Task. 
     * 
     * A task owns a number of activities, stored in the activities table, 
     * has claims on a number of resources, stored in the resource claimed table, 
     * has acquired a number of resources, stored in the resources owned table, 
     * and has a number of states, (blocked, terminated, aborted).
     * 
     * To execute activities, an algorithm asks the TaskTable to delegate the 
     * task to Task (this class), and this task dispatches the latest activity.
     */ 
public:
    Task(int id);

    int id();
    void increment_cycles_waiting(int current_cycle);

    void add_new_activity(Activity *activity);
    Activity *get_latest_activity();
    void do_latest_activity_delay_countdown();
    bool do_latest_activity(ResourceTable *resource_table, int cycle, bool should_check_safety);
    bool is_latest_activity_initiate();
    bool is_latest_activity_request();
    bool is_latest_activity_release();
    bool is_latest_activity_terminate();

    bool is_terminated();
    bool is_active();
    bool is_computing();
    bool terminate(int cycle);
    bool is_aborted();
    void abort(ResourceTable *resource_table);

    bool is_blocked();
    void block();
    void unblock();

    std::vector<int> generate_unmet_demand_vector();
    int check_unmet_demand_for_resource(int resource_id);

    void add_new_claim(Claim *claim);

    void add_resource_owned(Request *request);
    void release_resource_owned(Release *release);

    void print();
    std::string to_string();
    void print_finished_status();
    std::tuple<int, int> get_print_statistic();

private:
    bool initiate(Activity *latest_activity, ResourceTable *resource_table, bool should_check_safety);
    bool request(Activity *latest_activity, ResourceTable *resource_table, int cycle, bool should_check_safety);
    bool release(Activity *latest_activity, ResourceTable *resource_table);


    void set_latest_activity();
    bool execute_activity(Activity *latest_activity, ResourceTable *resource_table, int cycle, bool should_check_safety);
    void release_all_resources(ResourceTable *resource_table);
    bool determine_latest_activity_type(std::string target_type);
    bool is_request_legal(int request_resource_type, int request_count);

    void add_resource_owned(int resource_type, int request_count);
    void release_resource_owned(int resource_type, int release_count);


    int id_;
    int latest_activity_index_;
    bool blocked_;
    bool aborted_;
    bool terminated_;
    int initiation_cycle_;
    int termination_cycle_;
    int cycles_waiting_;
    int latest_cycle_waited_;
    std::map<int, Claim> resources_claimed_;
    std::map<int, int> resources_owned_;
    std::vector<Activity *> activities_table_;
};
} // namespace task

#endif