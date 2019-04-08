#ifndef HEADER_ACTIVITY
#define HEADER_ACTIVITY

#include <string>

namespace task
{
class Task;

class Activity
{

    /**
     * This is a base class for Initiate, Request, Release, and Termination. 
     */

  public:
    Activity(std::string type, int target_id, int delay, Task *task);
    virtual ~Activity() {}
    int get_target_id();
    void update_time_remaining_before_execute();
    void update_completion_state_after_execute(bool is_successful);
    bool is_active();
    bool is_computing();
    bool is_time_to_execute();
    void set_to_complete();
    virtual void print();

    std::string type();
    bool is_initiate();
    bool is_request();
    bool is_release();
    bool is_termination();

  protected:
    std::string type_;
    int target_id;
    task::Task *target_task_;
    int delay;
    int time_remaining_;
    bool has_begun_;
    bool completed_;

};
} // namespace task

#endif