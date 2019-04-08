#include <iostream>
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
    terminated_ = false;
    initiation_cycle_ = 0;
    cycles_waiting_ = 0;
    latest_cycle_waited_ = -1;

    claims_table_.insert(
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
        std::cout << "Not time to execute yet." << std::endl;
    }

    return is_successful;
}

bool Task::execute_activity(Activity *latest_activity, ResourceTable *resource_table, int cycle)
{
    if (latest_activity->is_initiate())
    {
        std::cout << "Initiating Task " << id_ << std::endl;
        return true;
    }
    if (latest_activity->is_request())
    {
        bool is_successful = resource_table->handle_new_request(static_cast<Request *>(latest_activity));
        
        if (!is_successful)
            increment_cycles_waiting(cycle);
            
        return is_successful;
    }
    else if (latest_activity->is_release())
    {
        resource_table->handle_new_release(static_cast<Release *>(latest_activity));
        return true;
    }
    else if (latest_activity->is_termination())
    {
        terminate(cycle);
        return true;
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

void Task::terminate(int cycle)
{
    terminated_ = true;
    termination_cycle_ = cycle;
    std::cout << "Terminating Task " << id_ << std::endl;
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
    for (int i = 0; i < latest_activity_index_; i++)
    {
        Activity *activity = activities_table_[i];
        if (activity->is_request())
        {
            resource_table->reverse_request(static_cast<Request *>(activity));
            activity->set_to_complete();
        }
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
    for (it = claims_table_.begin(); it != claims_table_.end(); it++)
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
        std::cout << total_time_spent << "   "
                  << cycles_waiting_ << "   "
                  << (100 * cycles_waiting_ / ((double)total_time_spent)) << "%"
                  << std::endl;
    }
    else
    {
        std::cout << "Aborted" << std::endl;
    }
}

} // namespace task