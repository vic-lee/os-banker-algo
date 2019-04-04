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

  protected:
    std::string type;
    int target_id;
    int delay;
    bool completed;

    void set_to_complete();

  public:
    Activity(std::string type, int target_id, int delay)
    {
        this->type = type;
        this->target_id = target_id;
        this->delay = delay;
        this->completed = false;
    }
    int get_target_id();
    void print();
};
} // namespace task

#endif