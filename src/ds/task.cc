#include <iostream>
#include <iomanip>
#include <math.h>
#include "task.h"
#include "claim.h"
#include "activity.h"
#include "activity_initiate.h"
#include "activity_request.h"
#include "activity_release.h"
#include "activity_initiate.h"

namespace task
{
Task::Task(int id)
{
    this->id_ = id;
    aborted_ = false;
    blocked_ = false;
    terminated_ = false;
    initiation_cycle_ = 0;
    cycles_waiting_ = 0;
    latest_cycle_waited_ = -1;
}

bool Task::do_latest_activity(ResourceTable *resource_table, int cycle, bool should_check_safety)
{
    /**
     * This function executes a task's latest activity (initiate, request, release, or terminate),
     * and returns a boolean describing whether an activity was successfully performed. 
     * 
     * If there's no activity remaining, the function terminates. 
     * Otherwise, if the task has no delay remaining (it is time to execute it), an activity 
     * is executed. The execution success status is returned. 
     */ 

    bool is_successful = false;

    set_latest_activity();

    if (latest_activity_index_ == -1)
        return is_successful;

    Activity *latest_activity = get_latest_activity();

    if (latest_activity->is_time_to_execute())
    {
        is_successful = execute_activity(latest_activity, resource_table, cycle, should_check_safety);
        latest_activity->update_completion_state_after_execute(is_successful);
    }

    return is_successful;
}

bool Task::execute_activity(Activity *latest_activity, ResourceTable *resource_table, int cycle, bool should_check_safety)
{
    /**
     * This function dispatches the latest activity. Dispatch is a polymorphic function 
     * for classes derived from the Activity class (Initiate, Release, Request, Terminate).
     */ 

    return latest_activity->dispatch(this, resource_table, should_check_safety, cycle);
}

Activity *Task::get_latest_activity()
{
    set_latest_activity();

    if (latest_activity_index_ == -1)
        return nullptr;

    // std::cout << "Latest activity type is " << activities_table_[latest_activity_index_]->type() << std::endl;

    return activities_table_[latest_activity_index_];
}

void Task::set_latest_activity()
{
    /**
     * This function finds the latest activity associated with a task by updating
     * the index of the latest activiy. 
     * 
     * -1 is the identifier for no active activities exist. 
     * 
     * The function is called in many places in the program to ensure that 
     * functions perform action on the latest activity. 
     */
     
    if (aborted_ || terminated_)
    {
        latest_activity_index_ = -1;
        return;
    }
    for (unsigned int i = 0; i < activities_table_.size(); i++)
    {
        if (activities_table_[i]->is_active())
        {
            latest_activity_index_ = i;
            return;
        }
    }
    latest_activity_index_ = -1;
}

void Task::add_new_activity(Activity *activity)
{
    activities_table_.push_back(activity);
}

void Task::do_latest_activity_delay_countdown()
{
    Activity *latest_activity = get_latest_activity();

    if (latest_activity != NULL)
        latest_activity->update_time_remaining_before_execute();
}

void Task::add_new_claim(Claim *claim)
{
    resources_claimed_.insert(
        std::pair<int, Claim>(claim->claimed_resource_id, *claim));
}

bool Task::is_aborted()
{
    return aborted_;
}

void Task::abort(ResourceTable *resource_table)
{
    std::cout << "Aborting Task " << id_ << std::endl;
    aborted_ = true;
    terminated_ = true;
    release_all_resources(resource_table);
}

void Task::release_all_resources(ResourceTable *resource_table)
{
    std::map<int, int>::iterator it;

    for (it = resources_owned_.begin(); it != resources_owned_.end(); it++)
    {
        int resource_type = it->first;
        int units_owned = it->second;
        Resource *target = resource_table->find_resource_by_id(resource_type);
        target->add_release_next_cycle(units_owned);

        std::cout << "Task " << id_ << " will release "
                  << units_owned << " of RT" << resource_type
                  << " next cycle" << std::endl;
    }
}

void Task::increment_cycles_waiting(int current_cycle)
{
    /* Only increment if this function has not been called this cycle */
    if (latest_cycle_waited_ == -1 || latest_cycle_waited_ < current_cycle)
    {
        latest_cycle_waited_ = current_cycle;
        cycles_waiting_++;
    }
    else if (latest_cycle_waited_ == current_cycle)
    {
        return;
    }
}

void Task::add_resource_owned(int resource_type, int request_count)
{
    if (resources_owned_.find(resource_type) == resources_owned_.end())
        resources_owned_.insert(std::pair<int, int>(resource_type, request_count));
    else
        resources_owned_.at(resource_type) += request_count;
}

void Task::add_resource_owned(Request *request)
{
    int resource_type = request->get_resource_type();
    int request_count = request->get_request_count();

    if (resources_owned_.find(resource_type) == resources_owned_.end())
        resources_owned_.insert(std::pair<int, int>(resource_type, request_count));
    else
        resources_owned_.at(resource_type) += request_count;
}

void Task::release_resource_owned(int resource_type, int release_count)
{
    resources_owned_.at(resource_type) -= release_count;
}

void Task::release_resource_owned(Release *release)
{
    int resource_type = release->get_resource_type();
    int release_count = release->get_release_count();
    resources_owned_.at(resource_type) -= release_count;
}

std::vector<int> Task::generate_unmet_demand_vector()
{
    std::vector<int> unmet_demand;

    for (unsigned int i = 0; i < resources_claimed_.size(); i++)
    {
        int resource_id = i + 1;
        int claim_count, own_count;

        claim_count = resources_claimed_.at(resource_id).claim_count;

        if (resources_owned_.find(resource_id) == resources_owned_.end())
        {
            own_count = 0;
        }
        else
        {
            own_count = resources_owned_.at(resource_id);
        }

        int max_addl_claim = claim_count - own_count;
        unmet_demand.push_back(max_addl_claim);
    }

    return unmet_demand;
}

int Task::check_unmet_demand_for_resource(int resource_id)
{
    std::vector<int> unmet_demand = generate_unmet_demand_vector();
    return unmet_demand[resource_id - 1];
}

bool Task::is_request_legal(int request_resource_type, int request_count)
{
    int max_addl_demand = check_unmet_demand_for_resource(request_resource_type);

    if (request_count > max_addl_demand)
    {
        // std::cout << "Task " << id_ << "'s request of RT"
        //           << request_resource_type << " is not legal;" << std::endl;
        return false;
    }
    return true;
}

bool Task::is_latest_activity_request()
{
    return determine_latest_activity_type("request");
}

bool Task::is_latest_activity_release()
{
    return determine_latest_activity_type("release");
}

bool Task::is_latest_activity_initiate()
{
    return determine_latest_activity_type("initiate");
}

bool Task::is_latest_activity_terminate()
{
    return determine_latest_activity_type("terminate");
}

bool Task::determine_latest_activity_type(std::string target_type)
{
    if (terminated_)
        return false;

    Activity *latest_actvity = get_latest_activity();

    if (latest_actvity == NULL)
        return false;

    if (target_type == "initiate")
        return latest_actvity->is_initiate();

    else if (target_type == "request")
        return latest_actvity->is_request();

    else if (target_type == "release")
        return latest_actvity->is_release();

    else if (target_type == "terminate")
        return latest_actvity->is_termination();

    return false;
}

int Task::id()
{
    return id_;
}

bool Task::is_terminated()
{
    return terminated_;
}

bool Task::is_active()
{
    return !terminated_;
}

bool Task::is_computing()
{
    Activity *activity = get_latest_activity();

    if (activity != NULL)
        return activity->is_computing();

    return false;
}

bool Task::is_blocked()
{
    return blocked_;
}

void Task::block()
{
    blocked_ = true;
}

void Task::unblock()
{
    blocked_ = false;
}

bool Task::terminate(int cycle)
{
    terminated_ = true;
    termination_cycle_ = cycle;
    // std::cout << "Terminating Task " << id_ << std::endl;
    return true;
}

void Task::print()
{
    std::cout << std::endl;

    std::cout << "Task ID: " << id_ << std::endl;

    std::cout << "---------------- Claims ---------------" << std::endl;
    std::map<int, Claim>::iterator it;
    for (it = resources_claimed_.begin(); it != resources_claimed_.end(); it++)
    {
        it->second.print();
    }

    std::cout << "------------- Activities --------------" << std::endl;
    // for (int i = 0; i < activities_table_.size(); i++)
    for (auto &activity : activities_table_)
    {
        activity->print();
    }
}

std::tuple<int, int> Task::get_print_statistic()
{
    if (is_aborted())
        return std::make_tuple(0, 0);

    int total_time_spent = termination_cycle_ - initiation_cycle_;
    return std::make_tuple(total_time_spent, cycles_waiting_);
}

void Task::print_finished_status()
{
    std::cout << "Task " << id_ << "\t    ";

    if (!is_aborted())
    {
        int total_time_spent = termination_cycle_ - initiation_cycle_;
        std::cout << std::setw(4) << total_time_spent
                  << std::setw(4) << cycles_waiting_
                  << std::setw(4) << (int)nearbyint(100 * cycles_waiting_ / ((double)total_time_spent)) << "%"
                  << std::endl;
    }
    else
    {
        std::cout << "Aborted" << std::endl;
    }
}

} // namespace task