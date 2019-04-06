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
    void add_new_activity(Activity *activity);
    void do_latest_activity(ResourceTable *resource_table, int cycle);
    std::string to_string();
    bool is_terminated();
    void terminate(int cycle);
    void print();
    std::tuple<int, int> get_print_statistic();
    void print_finished_status();
    int id();

  private:
    void set_latest_activity();
    Activity *get_latest_activity();
    bool execute_activity(Activity *latest_activity, ResourceTable *resource_table, int cycle);

    int id_;
    int latest_activity_index_;
    bool aborted_;
    bool terminated_;
    int initiation_cycle_;
    int termination_cycle_;
    int cycles_waiting_;
    std::map<int, Claim> claims_table_;
    std::vector<Activity *> activities_table_;
};
} // namespace task

#endif