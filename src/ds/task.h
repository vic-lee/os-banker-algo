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
  public:
    Task(int id);
    Task(int id, Claim claim);

    int id();
    void increment_cycles_waiting(int current_cycle);

    void add_new_activity(Activity *activity);
    Activity *get_latest_activity();
    bool do_latest_activity(ResourceTable *resource_table, int cycle);
    bool is_latest_activity_initiate();
    bool is_latest_activity_request();
    bool is_latest_activity_release();
    bool is_latest_activity_terminate();

    bool is_terminated();
    bool is_computing();
    void terminate(int cycle);
    bool is_aborted();
    void abort(ResourceTable *resource_table);

    bool is_blocked();
    void block();
    void unblock();

    std::vector<int> generate_unmet_demand_vector();
    int check_unmet_demand_for_resource(int resource_id);

    void print();
    std::string to_string();
    void print_finished_status();
    std::tuple<int, int> get_print_statistic();

  private:
    void set_latest_activity();
    bool execute_activity(Activity *latest_activity, ResourceTable *resource_table, int cycle);
    void release_resources(ResourceTable *resource_table);
    bool determine_latest_activity_type(std::string target_type);

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