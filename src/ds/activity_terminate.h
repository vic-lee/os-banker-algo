#ifndef HEADER_ACTIVITY_TERMINATION
#define HEADER_ACTIVITY_TERMINATION

#include <string>
#include "activity.h"

namespace task
{

class Termination : public Activity
{

    /**
     * The Termination class is a subclass of the Activity class. 
     * 
     * This class contains minimal information (other than the fact that the
     * task should terminate itself when this is its latest activity). All the 
     * specifics of terminating a task is better handled by the Task class itself.
     * 
     * As such, the dispatch function, overriden below, merely issues a callback 
     * to its target task (usually it is the task itself that calls this function).
     *  
     * We maintain the dispatch function here to maintain consistency across 
     * various Activity subclasses and to allow dispatch to be a polymorphic function. 
     */

public:
    Termination(int target_id, int delay)
        : Activity("terminate", target_id, delay) {}
    bool dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) override;
};

} // namespace task

#endif