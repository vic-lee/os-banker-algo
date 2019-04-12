#ifndef HEADER_ACTIVITY_INITIATE
#define HEADER_ACTIVITY_INITIATE

#include <string>
#include "activity.h"
#include "claim.h"

namespace task
{

    /**
     * The Initiate class is a subclass of the Activity class. 
     * 
     * Critically, one Initiate is responsible for initiating one claim
     * associated with a task. As such, this class has a data member that
     * stores a claim. See claim.h for the Claim class's functionalities. 
     * 
     * The dispatch function, overriden here, is responsible for carrying 
     * out the Initiate (i.e. registering a new claim in a task). The function 
     * checks if the claim is valid (i.e claim < max resource unit count)
     * if necessary. 
     */

class Initiate : public Activity
{
public:
    Initiate(int target_id, Claim claim);
    bool dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) override;
    Claim *claim();

private:
    Claim claim_;
};

} // namespace task

#endif