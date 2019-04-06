#ifndef HEADER_ACTIVITY
#define HEADER_ACTIVITY

#include <string>

namespace task
{
class Activity
{

    /**
     * This is a base class for Request, Release, and Termination. 
     * 
     * Initiate, the other possible Activity, does not have a derived
     * subclass. Initiation is done when we read in a file; therefore
     * there's no need for a subclass for relevant info and actions. 
     */

  public:
    Activity(std::string type, int target_id, int delay);
    virtual ~Activity() {}
    int get_target_id();
    void update_time_remaining_before_execute();
    void update_completion_state_after_execute(bool is_successful);
    bool is_active();
    bool is_time_to_execute();
    virtual void print();

    std::string type();
    bool is_initiate();
    bool is_request();
    bool is_release();
    bool is_termination();

  protected:
    std::string type_;
    int target_id;
    int delay;
    int time_remaining_;
    bool has_begun_;
    bool completed;

    void set_to_complete();
};
} // namespace task

#endif