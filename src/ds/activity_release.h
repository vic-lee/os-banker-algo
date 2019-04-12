#ifndef HEADER_ACTIVITY_RELEASE
#define HEADER_ACTIVITY_RELEASE

#include <string>
#include "activity.h"

namespace task
{
class ResourceTable;

class Release : public Activity
{

    /**
     * The Release class is a subclass of the Activity class. 
     * 
     * A release is defined if and only if resource release type and number
     * of units to release are specified. 
     * 
     * The dispatch function, overriden here, is responsible for carrying 
     * out the Release. This entails updating both the resource table and 
     * the task's resource ownership table. 
     */

public:
    Release(int target_id, int delay, int release_resource_type, int release_count);
    void print() override;
    int get_resource_type();
    int get_release_count();

    bool dispatch(Task *target_task, ResourceTable *resource_table, bool check_legal, int cycle) override;

private:
    int release_resource_type;
    int release_count;
};

} // namespace task

#endif