#include <iostream>
#include <iomanip>
#include <math.h>
#include "task.h"
#include "claim.h"
#include "activity.h"
#include "activity_request.h"
#include "activity_release.h"
#include "activity_initiate.h"

namespace task
{

Task::Task(int id, Claim claim)
{
    this->id_ = id;
    aborted_ = false;
    blocked_ = false;
    terminated_ = false;
    initiation_cycle_ = 0;
    cycles_waiting_ = 0;
    latest_cycle_waited_ = -1;

    resources_claimed_.insert(
        std::pair<int, Claim>(claim.claimed_resource_id, claim));
}

Task::Task(int id)
{
    this->id_ = id;
    aborted_ = false;
    terminated_ = false;
    initiation_cycle_ = 0;
    cycles_waiting_ = 0;
    latest_cycle_waited_ = -1;
}

void Task::set_latest_activity()
{
    if (aborted_ || terminated_)
    {
        latest_activity_index_ = -1;
        return;
    }

    for (int i = 0; i < activities_table_.size(); i++)
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

Activity *Task::get_latest_activity()
{
    set_latest_activity();

    if (latest_activity_index_ == -1)
        return nullptr;

    return activities_table_[latest_activity_index_];
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

    if (target_type == "initate")
        return latest_actvity->is_initiate();

    else if (target_type == "request")
        return latest_actvity->is_request();

    else if (target_type == "release")
        return latest_actvity->is_release();

    else if (target_type == "terminate")
        return latest_actvity->is_termination();

    return false;
}

bool Task::do_latest_activity(ResourceTable *resource_table, int cycle)
{
    bool is_successful = false;

    set_latest_activity();

    if (latest_activity_index_ == -1)
        return is_successful;

    Activity *latest_activity = get_latest_activity();

    latest_activity->update_time_remaining_before_execute();

    if (latest_activity->is_time_to_execute())
    {
        is_successful = execute_activity(latest_activity, resource_table, cycle);
        latest_activity->update_completion_state_after_execute(is_successful);
    }
    else
    {
        std::cout << "Not time to execute Task " << id_ << " yet." << std::endl;
    }

    return is_successful;
}

bool Task::execute_activity(Activity *latest_activity, ResourceTable *resource_table, int cycle)
{
    if (latest_activity->is_initiate())
    {
        return initiate();
    }
    if (latest_activity->is_request())
    {
        return request(latest_activity, resource_table, cycle);
    }
    else if (latest_activity->is_release())
    {
        return release(latest_activity, resource_table);
    }
    else if (latest_activity->is_termination())
    {
        return terminate(cycle);
    }
    
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
    if (blocked_)
        std::cout << "Debug warning: Process is already blocked before setting to block." << std::endl;

    blocked_ = true;
}

void Task::unblock()
{
    if (!blocked_)
        std::cout << "Debug warning: Process is already unblocked before disabling block." << std::endl;

    blocked_ = false;
}

bool Task::initiate()
{
    std::cout << "Initiating Task " << id_ << std::endl;
    return true;
}

bool Task::request(Activity *latest_activity, ResourceTable *resource_table, int cycle)
{
    bool can_satisfy_request = resource_table->handle_new_request(static_cast<Request *>(latest_activity));

    if (can_satisfy_request)
    {
        int resource_type = static_cast<Request *>(latest_activity)->get_resource_type();
        int request_count = static_cast<Request *>(latest_activity)->get_request_count();

        if (resources_owned_.find(resource_type) == resources_owned_.end())
            resources_owned_.insert(std::pair<int, int>(resource_type, request_count));
        else
            resources_owned_.at(resource_type) += request_count;
    }
    else
    {
        increment_cycles_waiting(cycle);
    }

    return can_satisfy_request;
}

bool Task::release(Activity *latest_activity, ResourceTable *resource_table)
{
    resource_table->handle_new_release(static_cast<Release *>(latest_activity));

    int resource_type = static_cast<Release *>(latest_activity)->get_resource_type();
    int release_count = static_cast<Release *>(latest_activity)->get_release_count();

    resources_owned_.at(resource_type) -= release_count;

    if (resources_owned_.at(resource_type) < 0)
        std::cout << "Debug: [negative resource ownership]; "
                  << "Own " << resources_owned_.at(resource_type)
                  << " of RT" << resource_type << std::endl;

    return true;
}

bool Task::terminate(int cycle)
{
    terminated_ = true;
    termination_cycle_ = cycle;
    std::cout << "Terminating Task " << id_ << std::endl;
    return true;
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
    release_resources(resource_table);
}

void Task::release_resources(ResourceTable *resource_table)
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
    std::cout << "Incrementing cycle for Task " << id_;
    if (latest_cycle_waited_ == -1 || latest_cycle_waited_ < current_cycle)
    {
        latest_cycle_waited_ = current_cycle;
        cycles_waiting_++;
        std::cout << " to " << cycles_waiting_ << std::endl;
    }
    else if (latest_cycle_waited_ == current_cycle)
    {
        std::cout << " to " << cycles_waiting_ << std::endl;
        return;
    }
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
    for (int i = 0; i < activities_table_.size(); i++)
    {
        activities_table_[i]->print();
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

std::vector<int> Task::generate_unmet_demand_vector()
{
    std::vector<int> unmet_demand;

    for (int i = 0; i < resources_claimed_.size(); i++)
    {
        int max_addl_claim = resources_claimed_.at(i).claim_count - resources_owned_.at(i);
        unmet_demand.push_back(max_addl_claim);
    }

    return unmet_demand;
}

int Task::check_unmet_demand_for_resource(int resource_id)
{
    std::vector<int> unmet_demand = generate_unmet_demand_vector();
    return unmet_demand[resource_id];
}

} // namespace task